#pragma once

#include "CLHelper.h"


bool InitCLFirstGPU(cl_device_id* di, cl_platform_id* pi, cl_context* ctx, cl_command_queue* queue)
{
	cl_device_id deviceId; 
	cl_platform_id platformId;
	
	cl_uint platformIdCount = 0;
	clGetPlatformIDs (0, nullptr, &platformIdCount);
	std::vector<cl_platform_id> platformIds (platformIdCount);
	clGetPlatformIDs(platformIdCount, platformIds.data(), nullptr);
	
	bool ok = false;
	
	for(int i = 0; i < platformIdCount; i++)
	{
		cl_uint deviceIdCount = 0;
		clGetDeviceIDs(platformIds[i], CL_DEVICE_TYPE_CPU, 0, nullptr, &deviceIdCount);
		if(deviceIdCount > 0)
		{
			std::vector<cl_device_id> deviceIds (deviceIdCount);
			clGetDeviceIDs (platformIds [0], CL_DEVICE_TYPE_CPU, deviceIdCount, deviceIds.data(), nullptr);
			deviceId = deviceIds[0];
			platformId = platformIds[i];
			ok = true; 
			break; 
		}
	}

	if(!ok)	return false; 
	
	
	cl_int err;
	*ctx = 0; 
	*queue = 0;	
	cl_context_properties props[3] = { CL_CONTEXT_PLATFORM, 0, 0 };
	props[1] = (cl_context_properties)platformId;
	*ctx = clCreateContext(props, 1, &deviceId, NULL, NULL, &err );
	if(err != CL_SUCCESS) return false; 
	*queue = clCreateCommandQueue(*ctx, deviceId, 0, &err);
	if(err != CL_SUCCESS) return false; 
    
	
	*pi = platformId;
	*di = deviceId; 
	return true; 
}

bool CreateCLProgram(std::string programSrc, cl_context ctx, cl_device_id deviceId, cl_program* clProg)
{
	cl_int err;
	const char* clProgramSrc = programSrc.c_str(); 
	const size_t clProgramSrcLen = programSrc.length();
	cl_program  clProgram = clCreateProgramWithSource(ctx, 1, (const char **)(&clProgramSrc), (const size_t *)&clProgramSrcLen, &err);	
	err = clBuildProgram(clProgram, 1, &deviceId, NULL, NULL, NULL);
	if (err == CL_BUILD_PROGRAM_FAILURE) 
	{
		// Determine the size of the log
		size_t logsize;
		clGetProgramBuildInfo(clProgram, deviceId, CL_PROGRAM_BUILD_LOG, 0, NULL, &logsize);

		// Allocate memory for the log
		std::vector<char> logBuffer(logsize);

		// Get the log
		clGetProgramBuildInfo(clProgram, deviceId, CL_PROGRAM_BUILD_LOG, logsize, &logBuffer[0], NULL);
		std::string log(logBuffer.begin(), logBuffer.end());
		
		// Print the log
		std::cout << log;
		
		return false; 
	}
	*clProg = clProgram;
	return true; 
	
}