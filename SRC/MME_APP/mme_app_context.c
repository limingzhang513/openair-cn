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


#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <arpa/inet.h>

#include "intertask_interface.h"
#include "mme_config.h"
#include "assertions.h"
#include "conversions.h"
#include "tree.h"
#include "enum_string.h"
#include "common_types.h"
#include "mme_app_extern.h"
#include "mme_app_ue_context.h"
#include "mme_app_defs.h"
#include "s1ap_mme.h"
#include "msc.h"
#include "dynamic_memory_check.h"
#include "log.h"
#include "gcc_diag.h"

//------------------------------------------------------------------------------
ue_context_t                           *
mme_create_new_ue_context (
  void)
{
  ue_context_t                           *new_p = CALLOC_CHECK (1, sizeof (ue_context_t));
  return new_p;
}

//------------------------------------------------------------------------------
void mme_app_ue_context_free_content (ue_context_t * const mme_ue_context_p)
{
  //  mme_app_imsi_t         imsi;
  //  unsigned               imsi_auth:1;
  //  enb_ue_s1ap_id_t       enb_ue_s1ap_id:24;
  //  mme_ue_s1ap_id_t       mme_ue_s1ap_id;
  //  uint32_t               ue_id;
  //  uint8_t                nb_of_vectors;
  //  eutran_vector_t       *vector_list;
  //  eutran_vector_t       *vector_in_use;
  //  unsigned               subscription_known:1;
  //  uint8_t                msisdn[MSISDN_LENGTH+1];
  //  uint8_t                msisdn_length;
  //  mm_state_t             mm_state;
  //  guti_t                 guti;
  //  me_identity_t          me_identity;
  //  ecgi_t                  e_utran_cgi;
  //  time_t                 cell_age;
  //  network_access_mode_t  access_mode;
  //  apn_config_profile_t   apn_profile;
  //  ard_t                  access_restriction_data;
  //  subscriber_status_t    sub_status;
  //  ambr_t                 subscribed_ambr;
  //  ambr_t                 used_ambr;
  //  rau_tau_timer_t        rau_tau_timer;
  //if (mme_ue_context_p->ue_radio_capabilities) FREE_CHECK(mme_ue_context_p->ue_radio_capabilities);
  // int                    ue_radio_cap_length;
  // Teid_t                 mme_s11_teid;
  // Teid_t                 sgw_s11_teid;
  // PAA_t                  paa;
  // char                   pending_pdn_connectivity_req_imsi[16];
  // uint8_t                pending_pdn_connectivity_req_imsi_length;
  FREE_OCTET_STRING(mme_ue_context_p->pending_pdn_connectivity_req_apn);
  FREE_OCTET_STRING(mme_ue_context_p->pending_pdn_connectivity_req_pdn_addr);
  // int                    pending_pdn_connectivity_req_pti;
  // unsigned               pending_pdn_connectivity_req_ue_id;
  // network_qos_t          pending_pdn_connectivity_req_qos;
  // pco_flat_t             pending_pdn_connectivity_req_pco;
  // DO NOT FREE THE FOLLOWING POINTER, IT IS esm_proc_data_t*
  // void                  *pending_pdn_connectivity_req_proc_data;
  //int                    pending_pdn_connectivity_req_request_type;
  //ebi_t                  default_bearer_id;
  //bearer_context_t       eps_bearers[BEARERS_PER_UE];

}

//------------------------------------------------------------------------------
ue_context_t                           *
mme_ue_context_exists_enb_ue_s1ap_id (
  mme_ue_context_t * const mme_ue_context_p,
  const enb_ue_s1ap_id_t enb_ue_s1ap_id)
{
  struct ue_context_s                    *ue_context_p = NULL;

  hashtable_ts_get (mme_ue_context_p->enb_ue_s1ap_id_ue_context_htbl, (const hash_key_t)enb_ue_s1ap_id, (void **)&ue_context_p);
  return ue_context_p;
}

//------------------------------------------------------------------------------
ue_context_t                           *
mme_ue_context_exists_mme_ue_s1ap_id (
  mme_ue_context_t * const mme_ue_context_p,
  const mme_ue_s1ap_id_t mme_ue_s1ap_id)
{
  hashtable_rc_t                          h_rc = HASH_TABLE_OK;
  void                                   *id = NULL;

  h_rc = hashtable_ts_get (mme_ue_context_p->mme_ue_s1ap_id_ue_context_htbl, (const hash_key_t)mme_ue_s1ap_id, (void **)&id);

  if (HASH_TABLE_OK == h_rc) {
    return mme_ue_context_exists_enb_ue_s1ap_id (mme_ue_context_p, (uintptr_t) id);
  }

  return NULL;
}
//------------------------------------------------------------------------------
struct ue_context_s                    *
mme_ue_context_exists_imsi (
  mme_ue_context_t * const mme_ue_context_p,
  const mme_app_imsi_t imsi)
{
  hashtable_rc_t                          h_rc = HASH_TABLE_OK;
  void                                   *id = NULL;
  uint64_t                                uint_imsi_key = 0;

  uint_imsi_key = mme_app_imsi_to_u64(imsi);

  h_rc = hashtable_ts_get (mme_ue_context_p->imsi_ue_context_htbl, (const hash_key_t)uint_imsi_key, (void **)&id);

  if (HASH_TABLE_OK == h_rc) {
    return mme_ue_context_exists_mme_ue_s1ap_id (mme_ue_context_p, (uintptr_t) id);
  }

  return NULL;
}

//------------------------------------------------------------------------------
struct ue_context_s                    *
mme_ue_context_exists_s11_teid (
  mme_ue_context_t * const mme_ue_context_p,
  const s11_teid_t teid)
{
  hashtable_rc_t                          h_rc = HASH_TABLE_OK;
  void                                   *id = NULL;

  h_rc = hashtable_ts_get (mme_ue_context_p->tun11_ue_context_htbl, (const hash_key_t)teid, (void **)&id);

  if (HASH_TABLE_OK == h_rc) {
    return mme_ue_context_exists_mme_ue_s1ap_id (mme_ue_context_p, (uintptr_t) id);
  }
  return NULL;
}

//------------------------------------------------------------------------------
ue_context_t                           *
mme_ue_context_exists_guti (
  mme_ue_context_t * const mme_ue_context_p,
  const guti_t * const guti_p)
{
  hashtable_rc_t                          h_rc = HASH_TABLE_OK;
  void                                   *id = NULL;

  h_rc = obj_hashtable_ts_get (mme_ue_context_p->guti_ue_context_htbl, (const void *)guti_p, sizeof (*guti_p), (void **)&id);

  if (HASH_TABLE_OK == h_rc) {
    return mme_ue_context_exists_mme_ue_s1ap_id (mme_ue_context_p, (uintptr_t)id);
  }

  return NULL;
}


//------------------------------------------------------------------------------
void
mme_ue_context_notified_new_ue_s1ap_id_association (
  mme_ue_context_t * const mme_ue_context_p,
  ue_context_t   * const ue_context_p,
  const enb_ue_s1ap_id_t enb_ue_s1ap_id,
  const mme_ue_s1ap_id_t mme_ue_s1ap_id)
{
  ue_context_t                           *same_ue_context_p = NULL;
  hashtable_rc_t                          h_rc = HASH_TABLE_OK;
  void                                   *id = NULL;

  if (INVALID_MME_UE_S1AP_ID == mme_ue_s1ap_id) {
    OAILOG_ERROR (LOG_MME_APP,
        "Error could not associate this ue context %p enb_ue_s1ap_ue_id "ENB_UE_S1AP_ID_FMT " with mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT "\n",
        ue_context_p, ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id);
    return;
  }

  if (ue_context_p->enb_ue_s1ap_id == enb_ue_s1ap_id) {
    // check again
    h_rc = hashtable_ts_get (mme_ue_context_p->enb_ue_s1ap_id_ue_context_htbl, (const hash_key_t)enb_ue_s1ap_id,  (void **)&same_ue_context_p);
    if (HASH_TABLE_OK == h_rc) {
      if (ue_context_p->mme_ue_s1ap_id != mme_ue_s1ap_id) {
        // new insertion of mme_ue_s1ap_id, not a change in the id
        h_rc = hashtable_ts_remove (mme_ue_context_p->mme_ue_s1ap_id_ue_context_htbl, (const hash_key_t)ue_context_p->mme_ue_s1ap_id,  (void **)&id);
        h_rc = hashtable_ts_insert (mme_ue_context_p->mme_ue_s1ap_id_ue_context_htbl, (const hash_key_t)mme_ue_s1ap_id, (void *)(uintptr_t)enb_ue_s1ap_id);
        if (HASH_TABLE_OK == h_rc) {
          OAILOG_DEBUG (LOG_MME_APP,
              "Associated this ue context %p, enb_ue_s1ap_ue_id " ENB_UE_S1AP_ID_FMT " with mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT "\n",
              ue_context_p, ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id);
          ue_context_p->mme_ue_s1ap_id = mme_ue_s1ap_id;

          s1ap_notified_new_ue_mme_s1ap_id_association (ue_context_p->sctp_assoc_id_key, enb_ue_s1ap_id, mme_ue_s1ap_id);
          return;
        }
      }
    }
  }
  OAILOG_ERROR (LOG_MME_APP,
      "Error could not associate this ue context %p, enb_ue_s1ap_ue_id " ENB_UE_S1AP_ID_FMT " with mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT " %s\n",
      ue_context_p, ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id, hashtable_rc_code2string(h_rc));
}
//------------------------------------------------------------------------------
void
mme_ue_context_update_coll_keys (
  mme_ue_context_t * const mme_ue_context_p,
  ue_context_t   * const ue_context_p,
  const enb_ue_s1ap_id_t enb_ue_s1ap_id,
  const mme_ue_s1ap_id_t mme_ue_s1ap_id,
  const mme_app_imsi_t   imsi,
  const s11_teid_t       mme_s11_teid,
  const guti_t   * const guti_p)  //  never NULL, if none put &ue_context_p->guti
{
  ue_context_t                           *same_ue_context_p = NULL;
  hashtable_rc_t                          h_rc = HASH_TABLE_OK;
  void                                   *id = NULL;
  enb_ue_s1ap_id_t                        old_enb_id = 0;
  uint64_t                                uint_imsi_key = 0;


  OAILOG_FUNC_IN(LOG_MME_APP);

  char                                    guti_str[GUTI2STR_MAX_LENGTH];

  GUTI2STR (&ue_context_p->guti, guti_str, GUTI2STR_MAX_LENGTH);
  OAILOG_DEBUG (LOG_MME_APP, "Update ue context.enb_ue_s1ap_id " ENB_UE_S1AP_ID_FMT " ue context.mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT " ue context.IMSI %" IMSI_FORMAT " ue context.GUTI %s\n",
             ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id, IMSI_DATA(ue_context_p->imsi), guti_str);
  GUTI2STR (guti_p, guti_str, GUTI2STR_MAX_LENGTH);
  OAILOG_DEBUG (LOG_MME_APP, "Update ue context %p enb_ue_s1ap_id " ENB_UE_S1AP_ID_FMT " mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT " IMSI %" IMSI_FORMAT " GUTI %s\n",
            ue_context_p, enb_ue_s1ap_id, mme_ue_s1ap_id, IMSI_DATA(imsi), guti_str);

  if (ue_context_p->enb_ue_s1ap_id != enb_ue_s1ap_id) {
    // warning: end of lock far from here!!!
    //pthread_mutex_lock(&mme_ue_context_p->enb_ue_s1ap_id_ue_context_htbl->mutex);
    old_enb_id = ue_context_p->enb_ue_s1ap_id;
    h_rc = hashtable_ts_remove (mme_ue_context_p->enb_ue_s1ap_id_ue_context_htbl, (const hash_key_t)old_enb_id,  (void **)&same_ue_context_p);
    h_rc = hashtable_ts_insert (mme_ue_context_p->enb_ue_s1ap_id_ue_context_htbl, (const hash_key_t)enb_ue_s1ap_id, (void *)same_ue_context_p);

    if (HASH_TABLE_OK != h_rc) {
      AssertFatal( 0 == 1, "Error could not update this ue context %p enb_ue_s1ap_id " ENB_UE_S1AP_ID_FMT " -> " ENB_UE_S1AP_ID_FMT " %s\n",
          ue_context_p, ue_context_p->enb_ue_s1ap_id, enb_ue_s1ap_id, hashtable_rc_code2string(h_rc));

      OAILOG_ERROR (LOG_MME_APP,
          "Error could not update this ue context %p enb_ue_s1ap_id " ENB_UE_S1AP_ID_FMT " -> " ENB_UE_S1AP_ID_FMT " %s\n",
          ue_context_p, ue_context_p->enb_ue_s1ap_id, enb_ue_s1ap_id, hashtable_rc_code2string(h_rc));
    }
    ue_context_p->enb_ue_s1ap_id = enb_ue_s1ap_id;

    // update other tables
    if (INVALID_MME_UE_S1AP_ID != ue_context_p->mme_ue_s1ap_id) {
      h_rc = hashtable_ts_remove (mme_ue_context_p->mme_ue_s1ap_id_ue_context_htbl, (const hash_key_t)ue_context_p->mme_ue_s1ap_id, (void **)&id);
    }
    if (INVALID_MME_UE_S1AP_ID != mme_ue_s1ap_id) {
      h_rc = hashtable_ts_insert (mme_ue_context_p->mme_ue_s1ap_id_ue_context_htbl, (const hash_key_t)ue_context_p->mme_ue_s1ap_id, (void *)(uintptr_t)enb_ue_s1ap_id);
    }
    //pthread_mutex_unlock(&mme_ue_context_p->enb_ue_s1ap_id_ue_context_htbl->mutex);
  }


  if (INVALID_MME_UE_S1AP_ID != mme_ue_s1ap_id) {
    if (ue_context_p->mme_ue_s1ap_id != mme_ue_s1ap_id) {
      // new insertion of mme_ue_s1ap_id, not a change in the id
      h_rc = hashtable_ts_remove (mme_ue_context_p->mme_ue_s1ap_id_ue_context_htbl, (const hash_key_t)ue_context_p->mme_ue_s1ap_id,  (void **)&id);
      h_rc = hashtable_ts_insert (mme_ue_context_p->mme_ue_s1ap_id_ue_context_htbl, (const hash_key_t)mme_ue_s1ap_id, (void *)(uintptr_t)enb_ue_s1ap_id);

      if (HASH_TABLE_OK != h_rc) {
        OAILOG_ERROR (LOG_MME_APP,
            "Error could not update this ue context %p enb_ue_s1ap_ue_id "ENB_UE_S1AP_ID_FMT " mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT " %s\n",
            ue_context_p, ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id, hashtable_rc_code2string(h_rc));
      }
      ue_context_p->mme_ue_s1ap_id = mme_ue_s1ap_id;
    }

    uint_imsi_key = mme_app_imsi_to_u64(imsi);

    h_rc = hashtable_ts_remove (mme_ue_context_p->imsi_ue_context_htbl, (const hash_key_t)uint_imsi_key, (void **)&id);
    h_rc = hashtable_ts_insert (mme_ue_context_p->imsi_ue_context_htbl, (const hash_key_t)uint_imsi_key, (void *)(uintptr_t)mme_ue_s1ap_id);

    h_rc = hashtable_ts_remove (mme_ue_context_p->tun11_ue_context_htbl, (const hash_key_t)ue_context_p->mme_s11_teid, (void **)&id);
    h_rc = hashtable_ts_insert (mme_ue_context_p->tun11_ue_context_htbl, (const hash_key_t)ue_context_p->mme_s11_teid, (void *)(uintptr_t)mme_ue_s1ap_id);


    char tmp[1024];
    int size = 1024;

    obj_hashtable_ts_dump_content (mme_ue_context_p->guti_ue_context_htbl, tmp, &size);
    OAILOG_ERROR (LOG_MME_APP,"%s\n", tmp);

    h_rc = obj_hashtable_ts_remove (mme_ue_context_p->guti_ue_context_htbl, (const void *const)&ue_context_p->guti, sizeof (ue_context_p->guti), (void **)&id);
    if (INVALID_MME_UE_S1AP_ID != mme_ue_s1ap_id) {
      h_rc = obj_hashtable_ts_insert (mme_ue_context_p->guti_ue_context_htbl, (const void *const)&ue_context_p->guti, sizeof (ue_context_p->guti), (void *)(uintptr_t)mme_ue_s1ap_id);
    }
    size = 1024;
    obj_hashtable_ts_dump_content (mme_ue_context_p->guti_ue_context_htbl, tmp, &size);
    OAILOG_ERROR (LOG_MME_APP,"%s\n", tmp);
  }

  if ((mme_app_imsi_compare(&(ue_context_p->imsi), &imsi) == false)
      || (ue_context_p->mme_ue_s1ap_id != mme_ue_s1ap_id)) {
      uint_imsi_key = mme_app_imsi_to_u64(imsi);
    h_rc = hashtable_ts_remove (mme_ue_context_p->imsi_ue_context_htbl, (const hash_key_t)uint_imsi_key, (void **)&id);
    if (INVALID_MME_UE_S1AP_ID != mme_ue_s1ap_id) {
      uint_imsi_key = mme_app_imsi_to_u64(imsi);
      h_rc = hashtable_ts_insert (mme_ue_context_p->imsi_ue_context_htbl, (const hash_key_t)uint_imsi_key, (void *)(uintptr_t)mme_ue_s1ap_id);
    } else {
      h_rc = HASH_TABLE_KEY_NOT_EXISTS;
    }
    if (HASH_TABLE_OK != h_rc) {
      OAILOG_DEBUG (LOG_MME_APP,
          "Error could not update this ue context %p enb_ue_s1ap_ue_id " ENB_UE_S1AP_ID_FMT " mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT " imsi %" IMSI_FORMAT ": %s\n",
          ue_context_p, ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id, IMSI_DATA(imsi), hashtable_rc_code2string(h_rc));
    }
    mme_app_copy_imsi(&ue_context_p->imsi, &imsi);
  }

  if ((ue_context_p->mme_s11_teid != mme_s11_teid)
      || (ue_context_p->mme_ue_s1ap_id != mme_ue_s1ap_id)) {
    h_rc = hashtable_ts_remove (mme_ue_context_p->tun11_ue_context_htbl, (const hash_key_t)ue_context_p->mme_s11_teid, (void **)&id);
    if (INVALID_MME_UE_S1AP_ID != mme_ue_s1ap_id) {
      h_rc = hashtable_ts_insert (mme_ue_context_p->tun11_ue_context_htbl, (const hash_key_t)mme_s11_teid, (void *)(uintptr_t)mme_ue_s1ap_id);
    } else {
      h_rc = HASH_TABLE_KEY_NOT_EXISTS;
    }

    if (HASH_TABLE_OK != h_rc) {
      OAILOG_DEBUG (LOG_MME_APP,
          "Error could not update this ue context %p enb_ue_s1ap_ue_id "ENB_UE_S1AP_ID_FMT " mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT " mme_s11_teid " TEID_FMT " : %s\n",
          ue_context_p, ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id, mme_s11_teid, hashtable_rc_code2string(h_rc));
    }
    ue_context_p->mme_s11_teid = mme_s11_teid;
  }

  if ((guti_p->gummei.mme_code != ue_context_p->guti.gummei.mme_code)
      || (guti_p->gummei.mme_gid != ue_context_p->guti.gummei.mme_gid)
      || (guti_p->m_tmsi != ue_context_p->guti.m_tmsi)
      || (guti_p->gummei.plmn.mcc_digit1 != ue_context_p->guti.gummei.plmn.mcc_digit1)
      || (guti_p->gummei.plmn.mcc_digit2 != ue_context_p->guti.gummei.plmn.mcc_digit2)
      || (guti_p->gummei.plmn.mcc_digit3 != ue_context_p->guti.gummei.plmn.mcc_digit3)
      || (ue_context_p->mme_ue_s1ap_id != mme_ue_s1ap_id)) {

    // may check guti_p with a kind of instanceof()?
    h_rc = obj_hashtable_ts_remove (mme_ue_context_p->guti_ue_context_htbl, &ue_context_p->guti, sizeof (*guti_p), (void **)&id);
    if (INVALID_MME_UE_S1AP_ID != mme_ue_s1ap_id) {
      h_rc = obj_hashtable_ts_insert (mme_ue_context_p->guti_ue_context_htbl, (const void *const)guti_p, sizeof (*guti_p), (void *)(uintptr_t)mme_ue_s1ap_id);
    } else {
      h_rc = HASH_TABLE_KEY_NOT_EXISTS;
    }

    if (HASH_TABLE_OK != h_rc) {
      char                                    guti_str[GUTI2STR_MAX_LENGTH];
      GUTI2STR (guti_p, guti_str, GUTI2STR_MAX_LENGTH);
      OAILOG_DEBUG (LOG_MME_APP, "Error could not update this ue context %p enb_ue_s1ap_ue_id "ENB_UE_S1AP_ID_FMT " mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT " guti %s: %s\n",
          ue_context_p, ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id, guti_str, hashtable_rc_code2string(h_rc));
    }
    memcpy(&ue_context_p->guti , guti_p, sizeof(ue_context_p->guti));
  }
  OAILOG_FUNC_OUT(LOG_MME_APP);
}


//------------------------------------------------------------------------------
int
mme_insert_ue_context (
  mme_ue_context_t * const mme_ue_context_p,
  const struct ue_context_s *const ue_context_p)
{
  hashtable_rc_t                          h_rc = HASH_TABLE_OK;
  uint64_t                                uint_imsi_key = 0;
  OAILOG_FUNC_IN (LOG_MME_APP);
  DevAssert (mme_ue_context_p );
  DevAssert (ue_context_p );


  // filled ENB UE S1AP ID
  h_rc = hashtable_ts_is_key_exists (mme_ue_context_p->enb_ue_s1ap_id_ue_context_htbl, (const hash_key_t)ue_context_p->enb_ue_s1ap_id);
  if (HASH_TABLE_OK == h_rc) {
    OAILOG_DEBUG (LOG_MME_APP, "This ue context %p already exists enb_ue_s1ap_id " ENB_UE_S1AP_ID_FMT "\n",
        ue_context_p, ue_context_p->enb_ue_s1ap_id);
    OAILOG_FUNC_RETURN (LOG_MME_APP, RETURNerror);
  }
  h_rc = hashtable_ts_insert (mme_ue_context_p->enb_ue_s1ap_id_ue_context_htbl,
                             (const hash_key_t)ue_context_p->enb_ue_s1ap_id,
                             (void *)ue_context_p);

  if (HASH_TABLE_OK != h_rc) {
    OAILOG_DEBUG (LOG_MME_APP, "Error could not register this ue context %p enb_ue_s1ap_id " ENB_UE_S1AP_ID_FMT " ue_id 0x%x\n",
        ue_context_p, ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id);
    OAILOG_FUNC_RETURN (LOG_MME_APP, RETURNerror);
  }


  if ( INVALID_MME_UE_S1AP_ID != ue_context_p->mme_ue_s1ap_id) {
    h_rc = hashtable_ts_is_key_exists (mme_ue_context_p->mme_ue_s1ap_id_ue_context_htbl, (const hash_key_t)ue_context_p->mme_ue_s1ap_id);

    if (HASH_TABLE_OK == h_rc) {
      OAILOG_DEBUG (LOG_MME_APP, "This ue context %p already exists mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT "\n",
          ue_context_p, ue_context_p->mme_ue_s1ap_id);
      OAILOG_FUNC_RETURN (LOG_MME_APP, RETURNerror);
    }
    //OAI_GCC_DIAG_OFF(discarded-qualifiers);
    h_rc = hashtable_ts_insert (mme_ue_context_p->mme_ue_s1ap_id_ue_context_htbl,
                                (const hash_key_t)ue_context_p->mme_ue_s1ap_id,
                                (void *)((uintptr_t)ue_context_p->enb_ue_s1ap_id));
    //OAI_GCC_DIAG_ON(discarded-qualifiers);

    if (HASH_TABLE_OK != h_rc) {
      OAILOG_DEBUG (LOG_MME_APP, "Error could not register this ue context %p mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT "\n",
          ue_context_p, ue_context_p->mme_ue_s1ap_id);
      OAILOG_FUNC_RETURN (LOG_MME_APP, RETURNerror);
    }

    // filled IMSI
    if (mme_app_is_imsi_empty(&(ue_context_p->imsi)) != false) {
      uint_imsi_key = mme_app_imsi_to_u64(ue_context_p->imsi);
      h_rc = hashtable_ts_insert (mme_ue_context_p->imsi_ue_context_htbl,
                                  (const hash_key_t)uint_imsi_key,
                                  (void *)((uintptr_t)ue_context_p->mme_ue_s1ap_id));

      if (HASH_TABLE_OK != h_rc) {
        OAILOG_DEBUG (LOG_MME_APP, "Error could not register this ue context %p mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT " imsi %" IMSI_FORMAT "\n",
            ue_context_p, ue_context_p->mme_ue_s1ap_id, IMSI_DATA(ue_context_p->imsi));
        OAILOG_FUNC_RETURN (LOG_MME_APP, RETURNerror);
      }
    }

    // filled S11 tun id
    if (ue_context_p->mme_s11_teid) {
      h_rc = hashtable_ts_insert (mme_ue_context_p->tun11_ue_context_htbl,
                                 (const hash_key_t)ue_context_p->mme_s11_teid,
                                 (void *)((uintptr_t)ue_context_p->mme_ue_s1ap_id));

      if (HASH_TABLE_OK != h_rc) {
        OAILOG_DEBUG (LOG_MME_APP, "Error could not register this ue context %p mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT " mme_s11_teid " TEID_FMT "\n",
            ue_context_p, ue_context_p->mme_ue_s1ap_id, ue_context_p->mme_s11_teid);
        OAILOG_FUNC_RETURN (LOG_MME_APP, RETURNerror);
      }
    }

    // filled guti
    if ((0 != ue_context_p->guti.gummei.mme_code) || (0 != ue_context_p->guti.gummei.mme_gid) || (0 != ue_context_p->guti.m_tmsi) || (0 != ue_context_p->guti.gummei.plmn.mcc_digit1) ||     // MCC 000 does not exist in ITU table
        (0 != ue_context_p->guti.gummei.plmn.mcc_digit2)
        || (0 != ue_context_p->guti.gummei.plmn.mcc_digit3)) {

      h_rc = obj_hashtable_ts_insert (mme_ue_context_p->guti_ue_context_htbl,
                                     (const void *const)&ue_context_p->guti,
                                     sizeof (ue_context_p->guti),
                                     (void *)((uintptr_t)ue_context_p->mme_ue_s1ap_id));

      if (HASH_TABLE_OK != h_rc) {
        char                                    guti_str[GUTI2STR_MAX_LENGTH];
        GUTI2STR (&ue_context_p->guti, guti_str, GUTI2STR_MAX_LENGTH);
        OAILOG_DEBUG (LOG_MME_APP, "Error could not register this ue context %p mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT " guti %s\n",
                ue_context_p, ue_context_p->mme_ue_s1ap_id, guti_str);
        OAILOG_FUNC_RETURN (LOG_MME_APP, RETURNerror);
      }
    }
  }

  /*
   * Updating statistics
   */
  __sync_fetch_and_add (&mme_ue_context_p->nb_ue_managed, 1);
  __sync_fetch_and_add (&mme_ue_context_p->nb_ue_since_last_stat, 1);

  OAILOG_FUNC_RETURN (LOG_MME_APP, RETURNok);
}


//------------------------------------------------------------------------------
void mme_notify_ue_context_released (
    mme_ue_context_t * const mme_ue_context_p,
    struct ue_context_s *ue_context_p)
{
  OAILOG_FUNC_IN (LOG_MME_APP);
  DevAssert (mme_ue_context_p );
  DevAssert (ue_context_p );
  /*
   * Updating statistics
   */
  __sync_fetch_and_sub (&mme_ue_context_p->nb_ue_managed, 1);
  __sync_fetch_and_sub (&mme_ue_context_p->nb_ue_since_last_stat, 1);

  // TODO HERE free resources

  OAILOG_FUNC_OUT (LOG_MME_APP);
}
//------------------------------------------------------------------------------
void mme_remove_ue_context (
  mme_ue_context_t * const mme_ue_context_p,
  struct ue_context_s *ue_context_p)
{
  unsigned int                           *id = NULL;
  hashtable_rc_t                          hash_rc = HASH_TABLE_OK;
  uint64_t                                uint_imsi_key = 0;
  OAILOG_FUNC_IN (LOG_MME_APP);
  DevAssert (mme_ue_context_p );
  DevAssert (ue_context_p );
  /*
   * Updating statistics
   */
  __sync_fetch_and_sub (&mme_ue_context_p->nb_ue_managed, 1);
  __sync_fetch_and_sub (&mme_ue_context_p->nb_ue_since_last_stat, 1);

  if (mme_app_is_imsi_empty(&(ue_context_p->imsi))) {
    uint_imsi_key = mme_app_imsi_to_u64(ue_context_p->imsi);
    hash_rc = hashtable_ts_remove (mme_ue_context_p->imsi_ue_context_htbl, (const hash_key_t)uint_imsi_key, (void **)&id);
    if (HASH_TABLE_OK != hash_rc)
      OAILOG_DEBUG(LOG_MME_APP, "UE context enb_ue_s1ap_ue_id "ENB_UE_S1AP_ID_FMT " mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT ", IMSI %" IMSI_FORMAT "  not in IMSI collection",
          ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id, IMSI_DATA(ue_context_p->imsi));
  }
  // filled NAS UE ID
  if (INVALID_MME_UE_S1AP_ID != ue_context_p->mme_ue_s1ap_id) {
    hash_rc = hashtable_ts_remove (mme_ue_context_p->mme_ue_s1ap_id_ue_context_htbl, (const hash_key_t)ue_context_p->mme_ue_s1ap_id, (void **)&id);
    if (HASH_TABLE_OK != hash_rc)
      OAILOG_DEBUG(LOG_MME_APP, "UE context enb_ue_s1ap_ue_id "ENB_UE_S1AP_ID_FMT ", mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT " not in MME UE S1AP ID collection",
          ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id);
  }
  // filled S11 tun id
  if (ue_context_p->mme_s11_teid) {
    hash_rc = hashtable_ts_remove (mme_ue_context_p->tun11_ue_context_htbl, (const hash_key_t)ue_context_p->mme_s11_teid, (void **)&id);
    if (HASH_TABLE_OK != hash_rc)
      OAILOG_DEBUG(LOG_MME_APP, "UE context enb_ue_s1ap_ue_id "ENB_UE_S1AP_ID_FMT " mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT ", MME S11 TEID  " TEID_FMT "  not in S11 collection",
          ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id, ue_context_p->mme_s11_teid);
  }
  // filled guti
  if ((ue_context_p->guti.gummei.mme_code) || (ue_context_p->guti.gummei.mme_gid) || (ue_context_p->guti.m_tmsi) ||
      (ue_context_p->guti.gummei.plmn.mcc_digit1) || (ue_context_p->guti.gummei.plmn.mcc_digit2) || (ue_context_p->guti.gummei.plmn.mcc_digit3)) { // MCC 000 does not exist in ITU table
    hash_rc = obj_hashtable_ts_remove (mme_ue_context_p->guti_ue_context_htbl, (const void *const)&ue_context_p->guti, sizeof (ue_context_p->guti), (void **)&id);
    if (HASH_TABLE_OK != hash_rc)
      OAILOG_DEBUG(LOG_MME_APP, "UE context enb_ue_s1ap_ue_id "ENB_UE_S1AP_ID_FMT " mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT ", GUTI  not in GUTI collection",
          ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id);
  }

  hash_rc = hashtable_ts_remove (mme_ue_context_p->enb_ue_s1ap_id_ue_context_htbl, (const hash_key_t)ue_context_p->enb_ue_s1ap_id, (void **)&ue_context_p);
  if (HASH_TABLE_OK != hash_rc)
    OAILOG_DEBUG(LOG_MME_APP, "UE context enb_ue_s1ap_ue_id "ENB_UE_S1AP_ID_FMT " mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT ", ENB_UE_S1AP_ID not ENB_UE_S1AP_ID collection",
      ue_context_p->enb_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id);

  mme_app_ue_context_free_content(ue_context_p);
  FREE_CHECK (ue_context_p);
  OAILOG_FUNC_OUT (LOG_MME_APP);
}

//------------------------------------------------------------------------------
bool
mme_app_dump_ue_context (
  const hash_key_t keyP,
  void *const ue_context_pP,
  void *unused_param_pP,
  void** unused_result_pP)
//------------------------------------------------------------------------------
{
  struct ue_context_s                    *const context_p = (struct ue_context_s *)ue_context_pP;
  uint8_t                                 j = 0;

  OAILOG_DEBUG (LOG_MME_APP, "-----------------------UE context %p --------------------\n", ue_context_pP);
  if (context_p) {
    OAILOG_DEBUG (LOG_MME_APP, "    - IMSI ...........: %" IMSI_FORMAT "\n", IMSI_DATA(context_p->imsi));
    OAILOG_DEBUG (LOG_MME_APP, "                        |  m_tmsi  | mmec | mmegid | mcc | mnc |\n");
    OAILOG_DEBUG (LOG_MME_APP, "    - GUTI............: | %08x |  %02x  |  %04x  | %03u | %03u |\n", context_p->guti.m_tmsi, context_p->guti.gummei.mme_code, context_p->guti.gummei.mme_gid,
                 /*
                  * TODO check if two or three digits MNC...
                  */
        context_p->guti.gummei.plmn.mcc_digit3 * 100 +
        context_p->guti.gummei.plmn.mcc_digit2 * 10 + context_p->guti.gummei.plmn.mcc_digit1,
        context_p->guti.gummei.plmn.mnc_digit3 * 100 + context_p->guti.gummei.plmn.mnc_digit2 * 10 + context_p->guti.gummei.plmn.mnc_digit1);
    OAILOG_DEBUG (LOG_MME_APP, "    - Authenticated ..: %s\n", (context_p->imsi_auth == IMSI_UNAUTHENTICATED) ? "FALSE" : "TRUE");
    OAILOG_DEBUG (LOG_MME_APP, "    - eNB UE s1ap ID .: %08x\n", context_p->enb_ue_s1ap_id);
    OAILOG_DEBUG (LOG_MME_APP, "    - MME UE s1ap ID .: %08x\n", context_p->mme_ue_s1ap_id);
    OAILOG_DEBUG (LOG_MME_APP, "    - MME S11 TEID ...: %08x\n", context_p->mme_s11_teid);
    OAILOG_DEBUG (LOG_MME_APP, "    - SGW S11 TEID ...: %08x\n", context_p->sgw_s11_teid);
    OAILOG_DEBUG (LOG_MME_APP, "                        | mcc | mnc | cell id  |\n");
    OAILOG_DEBUG (LOG_MME_APP, "    - E-UTRAN CGI ....: | %03u | %03u | %08x |\n",
                 context_p->e_utran_cgi.plmn.mcc_digit3 * 100 +
                 context_p->e_utran_cgi.plmn.mcc_digit2 * 10 +
                 context_p->e_utran_cgi.plmn.mcc_digit1,
                 context_p->e_utran_cgi.plmn.mnc_digit3 * 100 + context_p->e_utran_cgi.plmn.mnc_digit2 * 10 + context_p->e_utran_cgi.plmn.mnc_digit1,
                 context_p->e_utran_cgi.cell_identity);
    /*
     * Ctime return a \n in the string
     */
    OAILOG_DEBUG (LOG_MME_APP, "    - Last acquired ..: %s", ctime (&context_p->cell_age));

    /*
     * Display UE info only if we know them
     */
    if (SUBSCRIPTION_KNOWN == context_p->subscription_known) {
      OAILOG_DEBUG (LOG_MME_APP, "    - Status .........: %s\n", (context_p->sub_status == SS_SERVICE_GRANTED) ? "Granted" : "Barred");
#define DISPLAY_BIT_MASK_PRESENT(mASK)   \
    ((context_p->access_restriction_data & mASK) ? 'X' : 'O')
      OAILOG_DEBUG (LOG_MME_APP, "    (O = allowed, X = !O) |UTRAN|GERAN|GAN|HSDPA EVO|E_UTRAN|HO TO NO 3GPP|\n");
      OAILOG_DEBUG (LOG_MME_APP,
          "    - Access restriction  |  %c  |  %c  | %c |    %c    |   %c   |      %c      |\n",
          DISPLAY_BIT_MASK_PRESENT (ARD_UTRAN_NOT_ALLOWED),
          DISPLAY_BIT_MASK_PRESENT (ARD_GERAN_NOT_ALLOWED),
          DISPLAY_BIT_MASK_PRESENT (ARD_GAN_NOT_ALLOWED), DISPLAY_BIT_MASK_PRESENT (ARD_I_HSDPA_EVO_NOT_ALLOWED), DISPLAY_BIT_MASK_PRESENT (ARD_E_UTRAN_NOT_ALLOWED), DISPLAY_BIT_MASK_PRESENT (ARD_HO_TO_NON_3GPP_NOT_ALLOWED));
      OAILOG_DEBUG (LOG_MME_APP, "    - Access Mode ....: %s\n", ACCESS_MODE_TO_STRING (context_p->access_mode));
      OAILOG_DEBUG (LOG_MME_APP, "    - MSISDN .........: %-*s\n", MSISDN_LENGTH, context_p->msisdn);
      OAILOG_DEBUG (LOG_MME_APP, "    - RAU/TAU timer ..: %u\n", context_p->rau_tau_timer);
      OAILOG_DEBUG (LOG_MME_APP, "    - IMEISV .........: %*s\n", IMEISV_DIGITS_MAX, context_p->me_identity.imeisv);
      OAILOG_DEBUG (LOG_MME_APP, "    - AMBR (bits/s)     ( Downlink |  Uplink  )\n");
      OAILOG_DEBUG (LOG_MME_APP, "        Subscribed ...: (%010" PRIu64 "|%010" PRIu64 ")\n", context_p->subscribed_ambr.br_dl, context_p->subscribed_ambr.br_ul);
      OAILOG_DEBUG (LOG_MME_APP, "        Allocated ....: (%010" PRIu64 "|%010" PRIu64 ")\n", context_p->used_ambr.br_dl, context_p->used_ambr.br_ul);
      OAILOG_DEBUG (LOG_MME_APP, "    - Known vectors ..: %u\n", context_p->nb_of_vectors);

      for (j = 0; j < context_p->nb_of_vectors; j++) {
        int                                     k;
        char                                    xres_string[3 * XRES_LENGTH_MAX + 1];
        eutran_vector_t                        *vector_p;

        vector_p = &context_p->vector_list[j];
        OAILOG_DEBUG (LOG_MME_APP, "        - RAND ..: " RAND_FORMAT "\n", RAND_DISPLAY (vector_p->rand));
        OAILOG_DEBUG (LOG_MME_APP, "        - AUTN ..: " AUTN_FORMAT "\n", AUTN_DISPLAY (vector_p->autn));
        OAILOG_DEBUG (LOG_MME_APP, "        - KASME .: " KASME_FORMAT "\n", KASME_DISPLAY_1 (vector_p->kasme));
        OAILOG_DEBUG (LOG_MME_APP, "                   " KASME_FORMAT "\n", KASME_DISPLAY_2 (vector_p->kasme));

        for (k = 0; k < vector_p->xres.size; k++) {
          sprintf (&xres_string[k * 3], "%02x,", vector_p->xres.data[k]);
        }

        xres_string[k * 3 - 1] = '\0';
        OAILOG_DEBUG (LOG_MME_APP, "        - XRES ..: %s\n", xres_string);
      }

      OAILOG_DEBUG (LOG_MME_APP, "    - PDN List:\n");

      for (j = 0; j < context_p->apn_profile.nb_apns; j++) {
        struct apn_configuration_s             *apn_config_p;

        apn_config_p = &context_p->apn_profile.apn_configuration[j];
        /*
         * Default APN ?
         */
        OAILOG_DEBUG (LOG_MME_APP, "        - Default APN ...: %s\n", (apn_config_p->context_identifier == context_p->apn_profile.context_identifier)
                     ? "TRUE" : "FALSE");
        OAILOG_DEBUG (LOG_MME_APP, "        - APN ...........: %s\n", apn_config_p->service_selection);
        OAILOG_DEBUG (LOG_MME_APP, "        - AMBR (bits/s) ( Downlink |  Uplink  )\n");
        OAILOG_DEBUG (LOG_MME_APP, "                        (%010" PRIu64 "|%010" PRIu64 ")\n", apn_config_p->ambr.br_dl, apn_config_p->ambr.br_ul);
        OAILOG_DEBUG (LOG_MME_APP, "        - PDN type ......: %s\n", PDN_TYPE_TO_STRING (apn_config_p->pdn_type));
        OAILOG_DEBUG (LOG_MME_APP, "        - QOS\n");
        OAILOG_DEBUG (LOG_MME_APP, "            QCI .........: %u\n", apn_config_p->subscribed_qos.qci);
        OAILOG_DEBUG (LOG_MME_APP, "            Prio level ..: %u\n", apn_config_p->subscribed_qos.allocation_retention_priority.priority_level);
        OAILOG_DEBUG (LOG_MME_APP, "            Pre-emp vul .: %s\n", (apn_config_p->subscribed_qos.allocation_retention_priority.pre_emp_vulnerability == PRE_EMPTION_VULNERABILITY_ENABLED) ? "ENABLED" : "DISABLED");
        OAILOG_DEBUG (LOG_MME_APP, "            Pre-emp cap .: %s\n", (apn_config_p->subscribed_qos.allocation_retention_priority.pre_emp_capability == PRE_EMPTION_CAPABILITY_ENABLED) ? "ENABLED" : "DISABLED");

        if (apn_config_p->nb_ip_address == 0) {
          OAILOG_DEBUG (LOG_MME_APP, "            IP addr .....: Dynamic allocation\n");
        } else {
          int                                     i;

          OAILOG_DEBUG (LOG_MME_APP, "            IP addresses :\n");

          for (i = 0; i < apn_config_p->nb_ip_address; i++) {
            if (apn_config_p->ip_address[i].pdn_type == IPv4) {
              OAILOG_DEBUG (LOG_MME_APP, "                           [" IPV4_ADDR "]\n", IPV4_ADDR_DISPLAY_8 (apn_config_p->ip_address[i].address.ipv4_address));
            } else {
              char                                    ipv6[40];

              inet_ntop (AF_INET6, apn_config_p->ip_address[i].address.ipv6_address, ipv6, 40);
              OAILOG_DEBUG (LOG_MME_APP, "                           [%s]\n", ipv6);
            }
          }
        }
        OAILOG_DEBUG (LOG_MME_APP, "\n");
      }
      OAILOG_DEBUG (LOG_MME_APP, "    - Bearer List:\n");

      for (j = 0; j < BEARERS_PER_UE; j++) {
        bearer_context_t                       *bearer_context_p;

        bearer_context_p = &context_p->eps_bearers[j];

        if (bearer_context_p->s_gw_teid != 0) {
          OAILOG_DEBUG (LOG_MME_APP, "        Bearer id .......: %02u\n", j);
          OAILOG_DEBUG (LOG_MME_APP, "        S-GW TEID (UP)...: %08x\n", bearer_context_p->s_gw_teid);
          OAILOG_DEBUG (LOG_MME_APP, "        P-GW TEID (UP)...: %08x\n", bearer_context_p->p_gw_teid);
          OAILOG_DEBUG (LOG_MME_APP, "        QCI .............: %u\n", bearer_context_p->qci);
          OAILOG_DEBUG (LOG_MME_APP, "        Priority level ..: %u\n", bearer_context_p->prio_level);
          OAILOG_DEBUG (LOG_MME_APP, "        Pre-emp vul .....: %s\n", (bearer_context_p->pre_emp_vulnerability == PRE_EMPTION_VULNERABILITY_ENABLED) ? "ENABLED" : "DISABLED");
          OAILOG_DEBUG (LOG_MME_APP, "        Pre-emp cap .....: %s\n", (bearer_context_p->pre_emp_capability == PRE_EMPTION_CAPABILITY_ENABLED) ? "ENABLED" : "DISABLED");
        }
      }
    }
    OAILOG_DEBUG (LOG_MME_APP, "---------------------------------------------------------\n");
    return false;
  }
  OAILOG_DEBUG (LOG_MME_APP, "---------------------------------------------------------\n");
  return true;
}


//------------------------------------------------------------------------------
void
mme_app_dump_ue_contexts (
  const mme_ue_context_t * const mme_ue_context_p)
//------------------------------------------------------------------------------
{
  hashtable_ts_apply_callback_on_elements (mme_ue_context_p->enb_ue_s1ap_id_ue_context_htbl, mme_app_dump_ue_context, NULL, NULL);
}


//------------------------------------------------------------------------------
void
mme_app_handle_s1ap_ue_context_release_req (
  const itti_s1ap_ue_context_release_req_t const *s1ap_ue_context_release_req)
//------------------------------------------------------------------------------
{
  struct ue_context_s                    *ue_context_p = NULL;
  MessageDef                             *message_p = NULL;

  OAILOG_FUNC_IN (LOG_MME_APP);
  ue_context_p = mme_ue_context_exists_enb_ue_s1ap_id (&mme_app_desc.mme_ue_contexts, s1ap_ue_context_release_req->enb_ue_s1ap_id);

  if (!ue_context_p) {
    MSC_LOG_EVENT (MSC_MMEAPP_MME, "0 S1AP_UE_CONTEXT_RELEASE_REQ Unknown mme_ue_s1ap_id 0x%06" PRIX32 " ", s1ap_ue_context_release_req->mme_ue_s1ap_id);
    OAILOG_ERROR (LOG_MME_APP, "UE context doesn't exist for enb_ue_s1ap_ue_id "ENB_UE_S1AP_ID_FMT " mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT "\n",
        s1ap_ue_context_release_req->enb_ue_s1ap_id, s1ap_ue_context_release_req->mme_ue_s1ap_id);
    OAILOG_FUNC_OUT (LOG_MME_APP);
  }

  if ((ue_context_p->mme_s11_teid == 0) && (ue_context_p->sgw_s11_teid == 0)) {
    // no session was created, no need for releasing bearers in SGW
    message_p = itti_alloc_new_message (TASK_MME_APP, S1AP_UE_CONTEXT_RELEASE_COMMAND);
    AssertFatal (message_p , "itti_alloc_new_message Failed");
    memset ((void *)&message_p->ittiMsg.s1ap_ue_context_release_command, 0, sizeof (itti_s1ap_ue_context_release_command_t));
    S1AP_UE_CONTEXT_RELEASE_COMMAND (message_p).mme_ue_s1ap_id = ue_context_p->mme_ue_s1ap_id;
    S1AP_UE_CONTEXT_RELEASE_COMMAND (message_p).enb_ue_s1ap_id = ue_context_p->enb_ue_s1ap_id;
    MSC_LOG_TX_MESSAGE (MSC_MMEAPP_MME, MSC_S1AP_MME, NULL, 0, "0 S1AP_UE_CONTEXT_RELEASE_COMMAND mme_ue_s1ap_id %06" PRIX32 " ",
        S1AP_UE_CONTEXT_RELEASE_COMMAND (message_p).mme_ue_s1ap_id);
    itti_send_msg_to_task (TASK_S1AP, INSTANCE_DEFAULT, message_p);
  } else {
    mme_app_send_s11_release_access_bearers_req (ue_context_p);
  }
  OAILOG_FUNC_OUT (LOG_MME_APP);
}


/*
   From GPP TS 23.401 version 11.11.0 Release 11, section 5.3.5 S1 release procedure, point 6:
   The MME deletes any eNodeB related information ("eNodeB Address in Use for S1-MME" and "eNB UE S1AP
   ID") from the UE's MME context, but, retains the rest of the UE's MME context including the S-GW's S1-U
   configuration information (address and TEIDs). All non-GBR EPS bearers established for the UE are preserved
   in the MME and in the Serving GW.
   If the cause of S1 release is because of User is inactivity, Inter-RAT Redirection, the MME shall preserve the
   GBR bearers. If the cause of S1 release is because of CS Fallback triggered, further details about bearer handling
   are described in TS 23.272 [58]. Otherwise, e.g. Radio Connection With UE Lost, S1 signalling connection lost,
   eNodeB failure the MME shall trigger the MME Initiated Dedicated Bearer Deactivation procedure
   (clause 5.4.4.2) for the GBR bearer(s) of the UE after the S1 Release procedure is completed.
*/
//------------------------------------------------------------------------------
void
mme_app_handle_s1ap_ue_context_release_complete (
  const itti_s1ap_ue_context_release_complete_t const
  *s1ap_ue_context_release_complete)
//------------------------------------------------------------------------------
{
  struct ue_context_s                    *ue_context_p = NULL;

  OAILOG_FUNC_IN (LOG_MME_APP);
  ue_context_p = mme_ue_context_exists_enb_ue_s1ap_id (&mme_app_desc.mme_ue_contexts, s1ap_ue_context_release_complete->enb_ue_s1ap_id);

  if (!ue_context_p) {
    MSC_LOG_EVENT (MSC_MMEAPP_MME, "0 S1AP_UE_CONTEXT_RELEASE_COMPLETE Unknown mme_ue_s1ap_id 0x%06" PRIX32 " ", s1ap_ue_context_release_complete->mme_ue_s1ap_id);
    OAILOG_ERROR (LOG_MME_APP, "UE context doesn't exist for enb_ue_s1ap_ue_id "ENB_UE_S1AP_ID_FMT " mme_ue_s1ap_id " MME_UE_S1AP_ID_FMT "\n",
        s1ap_ue_context_release_complete->enb_ue_s1ap_id, s1ap_ue_context_release_complete->mme_ue_s1ap_id);
    OAILOG_FUNC_OUT (LOG_MME_APP);
  }

  mme_notify_ue_context_released(&mme_app_desc.mme_ue_contexts, ue_context_p);
  //mme_remove_ue_context(&mme_app_desc.mme_ue_contexts, ue_context_p);
  // TODO remove in context GBR bearers
  OAILOG_FUNC_OUT (LOG_MME_APP);
}
