/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "/mnt/c/Development/J2735/J2735_201603DA.ASN"
 * 	`asn1c -fcompound-names -D /mnt/c/Development/J2735/Gen`
 */

#ifndef	_PublicSafetyEventResponderWorkerType_H_
#define	_PublicSafetyEventResponderWorkerType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PublicSafetyEventResponderWorkerType {
	PublicSafetyEventResponderWorkerType_unavailable	= 0,
	PublicSafetyEventResponderWorkerType_towOperater	= 1,
	PublicSafetyEventResponderWorkerType_fireAndEMSWorker	= 2,
	PublicSafetyEventResponderWorkerType_aDOTWorker	= 3,
	PublicSafetyEventResponderWorkerType_lawEnforcement	= 4,
	PublicSafetyEventResponderWorkerType_hazmatResponder	= 5,
	PublicSafetyEventResponderWorkerType_animalControlWorker	= 6,
	PublicSafetyEventResponderWorkerType_otherPersonnel	= 7
	/*
	 * Enumeration is extensible
	 */
} e_PublicSafetyEventResponderWorkerType;

/* PublicSafetyEventResponderWorkerType */
typedef long	 PublicSafetyEventResponderWorkerType_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_PublicSafetyEventResponderWorkerType_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_PublicSafetyEventResponderWorkerType;
extern const asn_INTEGER_specifics_t asn_SPC_PublicSafetyEventResponderWorkerType_specs_1;
asn_struct_free_f PublicSafetyEventResponderWorkerType_free;
asn_struct_print_f PublicSafetyEventResponderWorkerType_print;
asn_constr_check_f PublicSafetyEventResponderWorkerType_constraint;
ber_type_decoder_f PublicSafetyEventResponderWorkerType_decode_ber;
der_type_encoder_f PublicSafetyEventResponderWorkerType_encode_der;
xer_type_decoder_f PublicSafetyEventResponderWorkerType_decode_xer;
xer_type_encoder_f PublicSafetyEventResponderWorkerType_encode_xer;
oer_type_decoder_f PublicSafetyEventResponderWorkerType_decode_oer;
oer_type_encoder_f PublicSafetyEventResponderWorkerType_encode_oer;
per_type_decoder_f PublicSafetyEventResponderWorkerType_decode_uper;
per_type_encoder_f PublicSafetyEventResponderWorkerType_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _PublicSafetyEventResponderWorkerType_H_ */
#include <asn_internal.h>