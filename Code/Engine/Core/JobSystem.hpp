#pragma once
#include <vector>
#include <thread>
#include <deque>
#include <mutex>
struct JobConfig
{
	int m_numWorkers = -1; //If negative number, create one per hardware core.
};
enum class JobStatus
{
	NEW,
	QUEUING,
	EXECUTING,
	COMPLETED,
	RETRIEVED
};
class Job
{
public:
	Job() {}
	virtual ~Job(){}
	virtual void Execute() = 0;
public:
	std::atomic<JobStatus> m_status = JobStatus::NEW;
	uint8_t m_jobFlag = 0;
};
class JobSystem;
class JobWorkerThread
{
	friend class JobSystem;
public:
	JobWorkerThread(int id, JobSystem* system);
	~JobWorkerThread();
	void ThreadMain();
private:
	int m_id = -1;
	uint8_t m_jobFlag = 0;
	JobSystem* m_system = nullptr;
	std::thread* m_thread = nullptr;
};

class JobSystem
{
	friend class JobWorkerThread;
public:
	JobSystem(JobConfig const& jobConfig);
	~JobSystem();
	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();
	void CreateWorkers(int numWorkers);
	void DestoryWorkers();
	void QueueJob(Job* jobToQueue);
	void CompleteJob(Job* jobToComplete);
	void RetrieveJob(Job* jobToRetrieve);
	Job* RetrieveJob();
	void RetrieveAllCompletedJobs();
	Job* ClaimJob(JobWorkerThread* owner);
	bool IsQuitting() const;
	void SetJobWokerThreadJobFlag(int workerId, uint8_t jobFlag);
protected:
	std::vector<JobWorkerThread*> m_workers;
	JobConfig m_config;
	std::atomic<bool> m_isQuitting = false;

	std::deque<Job*> m_queuedJobs;
	std::deque<Job*> m_executingJobs;
	std::deque<Job*> m_completedJobs;

	mutable std::mutex m_queuedJobsMutex;
	mutable std::mutex m_executingJobsMutex;
	mutable std::mutex m_completedJobsMutex;
};