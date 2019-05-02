/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "/mnt/c/Development/J2735/J2735_201603DA.ASN"
 * 	`asn1c -fcompound-names -D /mnt/c/Development/J2735/Gen`
 */

#ifndef	_RegionList_H_
#define	_RegionList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct RegionOffsets;

/* RegionList */
typedef struct RegionList {
	A_SEQUENCE_OF(struct RegionOffsets) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RegionList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RegionList;
extern asn_SET_OF_specifics_t asn_SPC_RegionList_specs_1;
extern asn_TYPE_member_t asn_MBR_RegionList_1[1];
extern asn_per_constraints_t asn_PER_type_RegionList_constr_1;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "RegionOffsets.h"

#endif	/* _RegionList_H_ */
#include <asn_internal.h>
