/*
 * Copyright (c) 2014-2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Generated by erpcgen 1.8.0 on Thu Oct  8 16:38:35 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#include "erpc_client_manager.h"
#include "erpc_port.h"
#include "erpc_codec.h"
extern "C"
{
#include "erpc_matrix_multiply.h"
}

#if 10800 != ERPC_VERSION_NUMBER
#error "The generated shim code version is different to the rest of eRPC code."
#endif

using namespace erpc;
using namespace std;

extern ClientManager *g_client;


// Constant variable definitions
#pragma weak matrix_size
extern const int32_t matrix_size = 5;


// MatrixMultiplyService interface erpcMatrixMultiply function client shim.
void erpcMatrixMultiply(Matrix matrix1, Matrix matrix2, Matrix result_matrix)
{
    erpc_status_t err = kErpcStatus_Success;

    // Get a new request.
    RequestContext request = g_client->createRequest(false);

    // Encode the request.
    Codec * codec = request.getCodec();

    if (codec == NULL)
    {
        err = kErpcStatus_MemoryError;
    }
    else
    {
        codec->startWriteMessage(kInvocationMessage, kMatrixMultiplyService_service_id, kMatrixMultiplyService_erpcMatrixMultiply_id, request.getSequence());

        for (uint32_t arrayCount0 = 0; arrayCount0 < 5; ++arrayCount0)
        {
            for (uint32_t arrayCount1 = 0; arrayCount1 < 5; ++arrayCount1)
            {
                codec->write(matrix1[arrayCount0][arrayCount1]);
            }
        }

        for (uint32_t arrayCount0 = 0; arrayCount0 < 5; ++arrayCount0)
        {
            for (uint32_t arrayCount1 = 0; arrayCount1 < 5; ++arrayCount1)
            {
                codec->write(matrix2[arrayCount0][arrayCount1]);
            }
        }

        // Send message to server
        // Codec status is checked inside this function.
        g_client->performRequest(request);

        for (uint32_t arrayCount0 = 0; arrayCount0 < 5; ++arrayCount0)
        {
            for (uint32_t arrayCount1 = 0; arrayCount1 < 5; ++arrayCount1)
            {
                codec->read(&result_matrix[arrayCount0][arrayCount1]);
            }
        }

        err = codec->getStatus();
    }

    // Dispose of the request.
    g_client->releaseRequest(request);

    // Invoke error handler callback function
    g_client->callErrorHandler(err, kMatrixMultiplyService_erpcMatrixMultiply_id);

    return;
}
