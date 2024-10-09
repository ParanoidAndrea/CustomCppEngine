#include "Engine/Core/JobSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
JobSystem::JobSystem(JobConfig const& jobConfig)
	:m_config(jobConfig)
{

}

JobSystem::~JobSystem()
{

}

void JobSystem::Startup()
{
	int numWorkers = m_config.m_numWorkers;
	if (numWorkers < 0)
	{
		numWorkers = std::thread::hardware_concurrency();
	}
	CreateWorkers(numWorkers);
}

void JobSystem::BeginFrame()
{
}

void JobSystem::EndFrame()
{
}

void JobSystem::Shutdown()
{
	m_isQuitting = true;
	DestoryWorkers();
}

void JobSystem::CreateWorkers(int numWorkers)
{
	for (int i = 0; i < numWorkers; ++i)
	{
		m_workers.push_back(new JobWorkerThread(i, this));
	}
}

void JobSystem::DestoryWorkers()
{
	for (size_t i = 0; i < m_workers.size(); ++i)
	{
		delete m_workers[i];
		m_workers[i] = nullptr;
	}
}

void JobSystem::QueueJob(Job* jobToQueue)
{
	m_queuedJobsMutex.lock();
	m_queuedJobs.push_back(jobToQueue);
	jobToQueue->m_status = JobStatus::QUEUING;
	m_queuedJobsMutex.unlock();
}

void JobSystem::CompleteJob(Job* jobToComplete)
{
	m_executingJobsMutex.lock();
	auto it = std::find(m_executingJobs.begin(), m_executingJobs.end(), jobToComplete);
	if (it != m_executingJobs.end())
	{
		m_executingJobs.erase(it);
		m_completedJobs.push_back(jobToComplete);
		jobToComplete->m_status = JobStatus::COMPLETED;
	}
	else
	{
		ERROR_RECOVERABLE("Can't find a job in the executing list to remove");
	}

	m_executingJobsMutex.unlock();
}

void JobSystem::RetrieveJob(Job* jobToRetrieve)
{
	m_completedJobsMutex.lock();
	for (auto it = m_completedJobs.begin(); it != m_completedJobs.end(); ++it)
	{
		if (*it == jobToRetrieve)
		{
			m_completedJobs.erase(it);
			jobToRetrieve->m_status = JobStatus::RETRIEVED;
			m_completedJobsMutex.unlock();
			return;
		}
	}
	m_completedJobsMutex.unlock();
	ERROR_RECOVERABLE("Can't find a job in the complete list to retrieve");


}

Job* JobSystem::RetrieveJob()
{
	m_completedJobsMutex.lock();
	if (!m_completedJobs.empty())
	{
		Job* jobToComplete = m_completedJobs.front();
		jobToComplete->m_status = JobStatus::RETRIEVED;
		m_completedJobs.pop_front();
		m_completedJobsMutex.unlock();
		return jobToComplete;
	}
	m_completedJobsMutex.unlock();
	return nullptr;
}

void JobSystem::RetrieveAllCompletedJobs()
{
	m_completedJobsMutex.lock();
	while (!m_completedJobs.empty())
	{
		Job* jobToComplete = m_completedJobs.front();
		jobToComplete->m_status = JobStatus::RETRIEVED;
		m_completedJobs.pop_front();
	}
	m_completedJobsMutex.unlock();
}

Job* JobSystem::ClaimJob(JobWorkerThread* owner)
{
	m_queuedJobsMutex.lock();
	for (auto it = m_queuedJobs.begin(); it != m_queuedJobs.end(); ++it)
	{
		Job* jobToClaim = *it;
		if (jobToClaim->m_jobFlag == owner->m_jobFlag)
		{
			{
				m_executingJobs.push_back(jobToClaim);
				jobToClaim->m_status = JobStatus::EXECUTING;
			}
			m_queuedJobs.erase(it);
			m_queuedJobsMutex.unlock();
			return jobToClaim;
		}
	}
	m_queuedJobsMutex.unlock();
	return nullptr;

}

bool JobSystem::IsQuitting() const
{
	return m_isQuitting;
}

void JobSystem::SetJobWokerThreadJobFlag(int workerId, uint8_t jobFlag)
{
	m_workers[workerId]->m_jobFlag = jobFlag;
}

JobWorkerThread::JobWorkerThread(int id, JobSystem* system)
	:m_id(id), m_system(system)
{
	m_thread = new std::thread(&JobWorkerThread::ThreadMain, this);
}

JobWorkerThread::~JobWorkerThread()
{
 	if (m_thread && m_thread->joinable()) 
	{
 		m_thread->join();
 	}
   delete m_thread;
   m_thread = nullptr;
}

void JobWorkerThread::ThreadMain()
{
	while (!m_system->m_isQuitting)
	{
		Job* jobToExcute = m_system->ClaimJob(this);
		if (jobToExcute&& !m_system->m_isQuitting)
		{
			jobToExcute->Execute();
			m_system->CompleteJob(jobToExcute);
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
}
