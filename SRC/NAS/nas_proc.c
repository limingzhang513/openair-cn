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
  Source      nas_proc.c

  Version     0.1

  Date        2012/09/20

  Product     NAS stack

  Subsystem   NAS main process

  Author      Frederic Maurel

  Description NAS procedure call manager

*****************************************************************************/

#include "nas_proc.h"
#include "log.h"

#include "emm_main.h"
#include "emm_sap.h"

#include "esm_main.h"
#include "esm_sap.h"
#include "msc.h"

#include <stdio.h>              // sprintf

/****************************************************************************/
/****************  E X T E R N A L    D E F I N I T I O N S  ****************/
/****************************************************************************/

/****************************************************************************/
/*******************  L O C A L    D E F I N I T I O N S  *******************/
/****************************************************************************/



/****************************************************************************/
/******************  E X P O R T E D    F U N C T I O N S  ******************/
/****************************************************************************/

/****************************************************************************
 **                                                                        **
 ** Name:    nas_proc_initialize()                                     **
 **                                                                        **
 ** Description:                                                           **
 **                                                                        **
 ** Inputs:  None                                                      **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     None                                                      **
 **      Return:    None                                       **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
void
nas_proc_initialize (
  mme_config_t * mme_config_p)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  /*
   * Initialize the EMM procedure manager
   */
  emm_main_initialize (mme_config_p);
  /*
   * Initialize the ESM procedure manager
   */
  esm_main_initialize ();
  OAILOG_FUNC_OUT (LOG_NAS_EMM);
}


/****************************************************************************
 **                                                                        **
 ** Name:    nas_proc_cleanup()                                        **
 **                                                                        **
 ** Description: Performs clean up procedure before the system is shutdown **
 **                                                                        **
 ** Inputs:  None                                                      **
 **          Others:    None                                       **
 **                                                                        **
 ** Outputs:     None                                                      **
 **          Return:    None                                       **
 **          Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
void
nas_proc_cleanup (
  void)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  /*
   * Perform the EPS Mobility Manager's clean up procedure
   */
  emm_main_cleanup ();
  /*
   * Perform the EPS Session Manager's clean up procedure
   */
  esm_main_cleanup ();
  OAILOG_FUNC_OUT (LOG_NAS_EMM);
}

/*
   --------------------------------------------------------------------------
            NAS procedures triggered by the user
   --------------------------------------------------------------------------
*/


/****************************************************************************
 **                                                                        **
 ** Name:    nas_proc_establish_ind()                                  **
 **                                                                        **
 ** Description: Processes the NAS signalling connection establishment     **
 **      indication message received from the network              **
 **                                                                        **
 ** Inputs:  ueid:      UE identifier                              **
 **      tac:       The code of the tracking area the initia-  **
 **             ting UE belongs to                         **
 **      data:      The initial NAS message transfered within  **
 **             the message                                **
 **      len:       The length of the initial NAS message      **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     None                                                      **
 **      Return:    RETURNok, RETURNerror                      **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
nas_proc_establish_ind (
  const enb_ue_s1ap_id_t enb_ue_s1ap_id_key,
  const mme_ue_s1ap_id_t ue_id,
  const tai_t tai,
  const ecgi_t cgi,
  const uint8_t * data,
  const size_t len)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     rc = RETURNerror;

  if (len > 0) {
    emm_sap_t                               emm_sap = {0};

    MSC_LOG_TX_MESSAGE (MSC_NAS_MME, MSC_NAS_EMM_MME, NULL, 0, "0 EMMAS_ESTABLISH_REQ ue id " MME_UE_S1AP_ID_FMT " tai:  plmn %c%c%c.%c%c%c tac %u",
        ue_id,
        (char)(tai.plmn.mcc_digit1 + 0x30), (char)(tai.plmn.mcc_digit2 + 0x30), (char)(tai.plmn.mcc_digit3 + 0x30),
        (char)(tai.plmn.mnc_digit1 + 0x30), (char)(tai.plmn.mnc_digit2 + 0x30),
        (9 < tai.plmn.mnc_digit3) ? ' ': (char)(tai.plmn.mnc_digit3 + 0x30),
        tai.tac);
    /*
     * Notify the EMM procedure call manager that NAS signalling
     * connection establishment indication message has been received
     * from the Access-Stratum sublayer
     */

    emm_sap.primitive = EMMAS_ESTABLISH_REQ;
    emm_sap.u.emm_as.u.establish.enb_ue_s1ap_id_key = enb_ue_s1ap_id_key;
    emm_sap.u.emm_as.u.establish.ue_id = ue_id;
    emm_sap.u.emm_as.u.establish.nas_msg.length = len;
    emm_sap.u.emm_as.u.establish.nas_msg.value = (uint8_t *) data;
    emm_sap.u.emm_as.u.establish.plmn_id = &tai.plmn;
    emm_sap.u.emm_as.u.establish.tac = tai.tac;
    rc = emm_sap_send (&emm_sap);
  }

  OAILOG_FUNC_RETURN (LOG_NAS_EMM, rc);
}

/****************************************************************************
 **                                                                        **
 ** Name:    nas_proc_dl_transfer_cnf()                                **
 **                                                                        **
 ** Description: Processes the downlink data transfer confirm message re-  **
 **      ceived from the network while NAS message has been succes-**
 **      sfully delivered to the NAS sublayer on the receiver side.**
 **                                                                        **
 ** Inputs:  ueid:      UE identifier                              **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     None                                                      **
 **      Return:    RETURNok, RETURNerror                      **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
nas_proc_dl_transfer_cnf (
  const uint32_t ue_id)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  emm_sap_t                               emm_sap = {0};
  int                                     rc = RETURNok;

  MSC_LOG_TX_MESSAGE (MSC_NAS_MME, MSC_NAS_EMM_MME, NULL, 0, "0 EMMAS_DATA_IND dl_transfer_conf ue id " MME_UE_S1AP_ID_FMT " ", ue_id);
  /*
   * Notify the EMM procedure call manager that downlink NAS message
   * has been successfully delivered to the NAS sublayer on the
   * receiver side
   */
  emm_sap.primitive = EMMAS_DATA_IND;
  emm_sap.u.emm_as.u.data.ue_id = ue_id;
  emm_sap.u.emm_as.u.data.delivered = true;
  emm_sap.u.emm_as.u.data.nas_msg.length = 0;
  rc = emm_sap_send (&emm_sap);
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, rc);
}

/****************************************************************************
 **                                                                        **
 ** Name:    nas_proc_dl_transfer_rej()                                **
 **                                                                        **
 ** Description: Processes the downlink data transfer confirm message re-  **
 **      ceived from the network while NAS message has not been    **
 **      delivered to the NAS sublayer on the receiver side.       **
 **                                                                        **
 ** Inputs:  ueid:      UE identifier                              **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     None                                                      **
 **      Return:    RETURNok, RETURNerror                      **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
nas_proc_dl_transfer_rej (
  const uint32_t ue_id)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  emm_sap_t                               emm_sap = {0};
  int                                     rc = RETURNok;

  MSC_LOG_TX_MESSAGE (MSC_NAS_MME, MSC_NAS_EMM_MME, NULL, 0, "0 EMMAS_DATA_IND dl_transfer_reject ue id " MME_UE_S1AP_ID_FMT " ", ue_id);
  /*
   * Notify the EMM procedure call manager that transmission
   * failure of downlink NAS message indication has been received
   * from lower layers
   */
  emm_sap.primitive = EMMAS_DATA_IND;
  emm_sap.u.emm_as.u.data.ue_id = ue_id;
  emm_sap.u.emm_as.u.data.delivered = false;
  emm_sap.u.emm_as.u.data.nas_msg.length = 0;
  rc = emm_sap_send (&emm_sap);
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, rc);
}

/****************************************************************************
 **                                                                        **
 ** Name:    nas_proc_ul_transfer_ind()                                **
 **                                                                        **
 ** Description: Processes uplink data transfer indication message recei-  **
 **      ved from the network                                      **
 **                                                                        **
 ** Inputs:  ueid:      UE identifier                              **
 **      data:      The transfered NAS message                 **
 **      len:       The length of the NAS message              **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     None                                                      **
 **      Return:    RETURNok, RETURNerror                      **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
nas_proc_ul_transfer_ind (
  const mme_ue_s1ap_id_t ue_id,
  const uint8_t * data,
  const size_t len)
{
  int                                     rc = RETURNerror;

  OAILOG_FUNC_IN (LOG_NAS_EMM);

  if (len > 0) {
    emm_sap_t                               emm_sap = {0};

    /*
     * Notify the EMM procedure call manager that data transfer
     * indication has been received from the Access-Stratum sublayer
     */
    MSC_LOG_TX_MESSAGE (MSC_NAS_MME, MSC_NAS_EMM_MME, NULL, 0, "0 EMMAS_DATA_IND ue id " MME_UE_S1AP_ID_FMT " len %u", ue_id, len);
    emm_sap.primitive = EMMAS_DATA_IND;
    emm_sap.u.emm_as.u.data.ue_id = ue_id;
    emm_sap.u.emm_as.u.data.delivered = true;
    emm_sap.u.emm_as.u.data.nas_msg.length = len;
    emm_sap.u.emm_as.u.data.nas_msg.value = (uint8_t *) data;
    rc = emm_sap_send (&emm_sap);
  }

  OAILOG_FUNC_RETURN (LOG_NAS_EMM, rc);
}

//------------------------------------------------------------------------------
int
nas_proc_auth_param_res (
  emm_cn_auth_res_t * emm_cn_auth_res)
{
  int                                     rc = RETURNerror;
  emm_sap_t                               emm_sap = {0};

  OAILOG_FUNC_IN (LOG_NAS_EMM);
  MSC_LOG_TX_MESSAGE (MSC_NAS_MME, MSC_NAS_EMM_MME, NULL, 0, "0 EMMCN_AUTHENTICATION_PARAM_RES");
  emm_sap.primitive = EMMCN_AUTHENTICATION_PARAM_RES;
  emm_sap.u.emm_cn.u.auth_res = emm_cn_auth_res;
  rc = emm_sap_send (&emm_sap);
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, rc);
}

//------------------------------------------------------------------------------
int
nas_proc_auth_param_fail (
  emm_cn_auth_fail_t * emm_cn_auth_fail)
{
  int                                     rc = RETURNerror;
  emm_sap_t                               emm_sap = {0};

  OAILOG_FUNC_IN (LOG_NAS_EMM);
  MSC_LOG_TX_MESSAGE (MSC_NAS_MME, MSC_NAS_EMM_MME, NULL, 0, "0 EMMCN_AUTHENTICATION_PARAM_FAIL");
  emm_sap.primitive = EMMCN_AUTHENTICATION_PARAM_FAIL;
  emm_sap.u.emm_cn.u.auth_fail = emm_cn_auth_fail;
  rc = emm_sap_send (&emm_sap);
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, rc);
}

//------------------------------------------------------------------------------
int
nas_proc_deregister_ue (
    mme_ue_s1ap_id_t ue_id)
{
  int                                     rc = RETURNerror;
  emm_sap_t                               emm_sap = {0};

  OAILOG_FUNC_IN (LOG_NAS_EMM);
  MSC_LOG_TX_MESSAGE (MSC_NAS_MME, MSC_NAS_EMM_MME, NULL, 0, "0 EMMCN_DEREGISTER_UE ue_id " MME_UE_S1AP_ID_FMT " ", ue_id);
  emm_sap.primitive = EMMCN_DEREGISTER_UE;
  emm_sap.u.emm_cn.u.deregister.ue_id = ue_id;
  rc = emm_sap_send (&emm_sap);
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, rc);
}

//------------------------------------------------------------------------------
int
nas_proc_pdn_connectivity_res (
  emm_cn_pdn_res_t * emm_cn_pdn_res)
{
  int                                     rc = RETURNerror;
  emm_sap_t                               emm_sap = {0};

  OAILOG_FUNC_IN (LOG_NAS_EMM);
  MSC_LOG_TX_MESSAGE (MSC_NAS_MME, MSC_NAS_EMM_MME, NULL, 0, "0 EMMCN_PDN_CONNECTIVITY_RES");
  emm_sap.primitive = EMMCN_PDN_CONNECTIVITY_RES;
  emm_sap.u.emm_cn.u.emm_cn_pdn_res = emm_cn_pdn_res;
  rc = emm_sap_send (&emm_sap);
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, rc);
}

//------------------------------------------------------------------------------
int
nas_proc_pdn_connectivity_fail (
  emm_cn_pdn_fail_t * emm_cn_pdn_fail)
{
  int                                     rc = RETURNerror;
  emm_sap_t                               emm_sap = {0};

  OAILOG_FUNC_IN (LOG_NAS_EMM);
  MSC_LOG_TX_MESSAGE (MSC_NAS_MME, MSC_NAS_EMM_MME, NULL, 0, "0 EMMCN_PDN_CONNECTIVITY_FAIL");
  emm_sap.primitive = EMMCN_PDN_CONNECTIVITY_FAIL;
  emm_sap.u.emm_cn.u.emm_cn_pdn_fail = emm_cn_pdn_fail;
  rc = emm_sap_send (&emm_sap);
  OAILOG_FUNC_RETURN (LOG_NAS_EMM, rc);
}

/****************************************************************************/
/*********************  L O C A L    F U N C T I O N S  *********************/
/****************************************************************************/
