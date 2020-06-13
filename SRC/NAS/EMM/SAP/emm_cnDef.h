/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under 
 * the Apache License, Version 2.0  (the "License"); you may not use this file
 * except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*****************************************************************************

Source      emm_cnDef.h

Version     0.1

Date        2013/12/05

Product     NAS stack

Subsystem   EPS Core Network

Author      Sebastien Roux, Lionel GAUTHIER

Description

*****************************************************************************/


#ifndef FILE_EMM_CNDEF_SEEN
#define FILE_EMM_CNDEF_SEEN
#include "intertask_interface.h"

typedef enum emmcn_primitive_s {
  _EMMCN_START = 400,
  _EMMCN_AUTHENTICATION_PARAM_RES,
  _EMMCN_AUTHENTICATION_PARAM_FAIL,
  _EMMCN_DEREGISTER_UE,
  _EMMCN_PDN_CONNECTIVITY_RES, // LG
  _EMMCN_PDN_CONNECTIVITY_FAIL,// LG
  _EMMCN_END
} emm_cn_primitive_t;

typedef itti_nas_auth_param_rsp_t        emm_cn_auth_res_t;
typedef itti_nas_auth_param_fail_t       emm_cn_auth_fail_t;
typedef itti_nas_pdn_connectivity_rsp_t  emm_cn_pdn_res_t;
typedef itti_nas_pdn_connectivity_fail_t emm_cn_pdn_fail_t;

typedef struct emm_cn_deregister_ue_s {
  uint32_t ue_id;
} emm_cn_deregister_ue_t;

typedef struct emm_mme_ul_s {
  emm_cn_primitive_t primitive;
  union {
    emm_cn_auth_res_t       *auth_res;
    emm_cn_auth_fail_t      *auth_fail;
    emm_cn_deregister_ue_t   deregister;
    emm_cn_pdn_res_t        *emm_cn_pdn_res;
    emm_cn_pdn_fail_t       *emm_cn_pdn_fail;
  } u;
} emm_cn_t;

#endif /* FILE_EMM_CNDEF_SEEN */
