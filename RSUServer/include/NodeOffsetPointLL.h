/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "/mnt/c/Development/J2735/J2735_201603DA.ASN"
 * 	`asn1c -fcompound-names -D /mnt/c/Development/J2735/Gen`
 */

#ifndef	_NodeOffsetPointLL_H_
#define	_NodeOffsetPointLL_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Node-LL-24B.h"
#include "Node-LL-28B.h"
#include "Node-LL-32B.h"
#include "Node-LL-36B.h"
#include "Node-LL-44B.h"
#include "Node-LL-48B.h"
#include "Node-LLmD-64b.h"
#include "RegionalExtension.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum NodeOffsetPointLL_PR {
	NodeOffsetPointLL_PR_NOTHING,	/* No components present */
	NodeOffsetPointLL_PR_node_LL1,
	NodeOffsetPointLL_PR_node_LL2,
	NodeOffsetPointLL_PR_node_LL3,
	NodeOffsetPointLL_PR_node_LL4,
	NodeOffsetPointLL_PR_node_LL5,
	NodeOffsetPointLL_PR_node_LL6,
	NodeOffsetPointLL_PR_node_LatLon,
	NodeOffsetPointLL_PR_regional
} NodeOffsetPointLL_PR;

/* NodeOffsetPointLL */
typedef struct NodeOffsetPointLL {
	NodeOffsetPointLL_PR present;
	union NodeOffsetPointLL_u {
		Node_LL_24B_t	 node_LL1;
		Node_LL_28B_t	 node_LL2;
		Node_LL_32B_t	 node_LL3;
		Node_LL_36B_t	 node_LL4;
		Node_LL_44B_t	 node_LL5;
		Node_LL_48B_t	 node_LL6;
		Node_LLmD_64b_t	 node_LatLon;
		RegionalExtension_124P0_t	 regional;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} NodeOffsetPointLL_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_NodeOffsetPointLL;
extern asn_CHOICE_specifics_t asn_SPC_NodeOffsetPointLL_specs_1;
extern asn_TYPE_member_t asn_MBR_NodeOffsetPointLL_1[8];
extern asn_per_constraints_t asn_PER_type_NodeOffsetPointLL_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _NodeOffsetPointLL_H_ */
#include <asn_internal.h>
