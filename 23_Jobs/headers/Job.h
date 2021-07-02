#pragma once

#include "global.h"

class Job
{
private:
	
	// Check process creation status 
	BOOL m_process_creation_status = FALSE;
	
	// Check process is in a job or not
	BOOL m_already_process_in_job = FALSE;

	// It store handle of the job object
	HANDLE m_handle_job_object = nullptr;

	// Set information about startup of the new spawned process
	STARTUPINFO m_startup_info = { 0 };
	
	// Get information about new spawned process
	PROCESS_INFORMATION m_process_info;

	// Get restriction information about job object execution mode
	JOBOBJECT_BASIC_LIMIT_INFORMATION m_limited_job_object_info = { 0 };

	// Set restriction information about job object ui mode
	JOBOBJECT_BASIC_UI_RESTRICTIONS m_limitied_job_object_ui = { 0 };

	// Used to retrieve information about job ui 
	JOBOBJECT_BASIC_UI_RESTRICTIONS m_get_information_job_object_ui = { 0 };

	// Used to query accounting information of job object
	JOBOBJECT_BASIC_ACCOUNTING_INFORMATION m_basic_job_object_info = { 0 };

public:
	// Constructor of the Job Object
	Job();

	// Deconstructor of the Job Object
	~Job();

	// Check the process is in a job or not.
	BOOL Check();

	// Set some basic restriction for the job object
	BOOL RestrictedInformation();

	// Set some basic UI restriction for the job object
	BOOL RestrictedUi();

	// Get information from the job
	BOOL GetJobInformation();

	// Spawn process and set it to the job
	BOOL SpawnProcessInJob();

	// Check new spawned process has in a job or not.
	BOOL CheckProcessInJob();

	// Display job information and it is usage
	VOID DisplayInformation();
};
