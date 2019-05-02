/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "/mnt/c/Development/J2735/J2735_201603DA.ASN"
 * 	`asn1c -fcompound-names -D /mnt/c/Development/J2735/Gen`
 */

#ifndef	_VehicleStatus_H_
#define	_VehicleStatus_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ExteriorLights.h"
#include "LightbarInUse.h"
#include "BrakeAppliedPressure.h"
#include "CoefficientOfFriction.h"
#include "SunSensor.h"
#include "RainSensor.h"
#include "AmbientAirTemperature.h"
#include "AmbientAirPressure.h"
#include "ThrottlePosition.h"
#include "SpeedConfidence.h"
#include "GNSSstatus.h"
#include "SteeringWheelAngle.h"
#include "SteeringWheelAngleConfidence.h"
#include "SteeringWheelAngleRateOfChange.h"
#include "DrivingWheelAngle.h"
#include <constr_SEQUENCE.h>
#include "VerticalAccelerationThreshold.h"
#include "YawRateConfidence.h"
#include "AccelerationConfidence.h"
#include "ObstacleDistance.h"
#include "DSRC_Angle.h"
#include "DDateTime.h"
#include "VehicleHeight.h"
#include "BumperHeights.h"
#include "VehicleMass.h"
#include "TrailerWeight.h"
#include "VehicleType.h"
#include "EssPrecipYesNo.h"
#include "EssPrecipRate.h"
#include "EssPrecipSituation.h"
#include "EssSolarRadiation.h"
#include "EssMobileFriction.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct WiperSet;
struct BrakeSystemStatus;
struct FullPositionVector;
struct SpeedandHeadingandThrottleConfidence;
struct VehicleIdent;
struct J1939data;
struct AccelerationSet4Way;
struct ConfidenceSet;

/* VehicleStatus */
typedef struct VehicleStatus {
	ExteriorLights_t	*lights	/* OPTIONAL */;
	LightbarInUse_t	*lightBar	/* OPTIONAL */;
	struct WiperSet	*wipers	/* OPTIONAL */;
	struct BrakeSystemStatus	*brakeStatus	/* OPTIONAL */;
	BrakeAppliedPressure_t	*brakePressure	/* OPTIONAL */;
	CoefficientOfFriction_t	*roadFriction	/* OPTIONAL */;
	SunSensor_t	*sunData	/* OPTIONAL */;
	RainSensor_t	*rainData	/* OPTIONAL */;
	AmbientAirTemperature_t	*airTemp	/* OPTIONAL */;
	AmbientAirPressure_t	*airPres	/* OPTIONAL */;
	struct VehicleStatus__steering {
		SteeringWheelAngle_t	 angle;
		SteeringWheelAngleConfidence_t	*confidence	/* OPTIONAL */;
		SteeringWheelAngleRateOfChange_t	*rate	/* OPTIONAL */;
		DrivingWheelAngle_t	*wheels	/* OPTIONAL */;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *steering;
	struct VehicleStatus__accelSets {
		struct AccelerationSet4Way	*accel4way	/* OPTIONAL */;
		VerticalAccelerationThreshold_t	*vertAccelThres	/* OPTIONAL */;
		YawRateConfidence_t	*yawRateCon	/* OPTIONAL */;
		AccelerationConfidence_t	*hozAccelCon	/* OPTIONAL */;
		struct ConfidenceSet	*confidenceSet	/* OPTIONAL */;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *accelSets;
	struct VehicleStatus__object {
		ObstacleDistance_t	 obDist;
		DSRC_Angle_t	 obDirect;
		DDateTime_t	 dateTime;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *object;
	struct FullPositionVector	*fullPos	/* OPTIONAL */;
	ThrottlePosition_t	*throttlePos	/* OPTIONAL */;
	struct SpeedandHeadingandThrottleConfidence	*speedHeadC	/* OPTIONAL */;
	SpeedConfidence_t	*speedC	/* OPTIONAL */;
	struct VehicleStatus__vehicleData {
		VehicleHeight_t	 height;
		BumperHeights_t	 bumpers;
		VehicleMass_t	 mass;
		TrailerWeight_t	 trailerWeight;
		VehicleType_t	 type;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *vehicleData;
	struct VehicleIdent	*vehicleIdent	/* OPTIONAL */;
	struct J1939data	*j1939data	/* OPTIONAL */;
	struct VehicleStatus__weatherReport {
		EssPrecipYesNo_t	 isRaining;
		EssPrecipRate_t	*rainRate	/* OPTIONAL */;
		EssPrecipSituation_t	*precipSituation	/* OPTIONAL */;
		EssSolarRadiation_t	*solarRadiation	/* OPTIONAL */;
		EssMobileFriction_t	*friction	/* OPTIONAL */;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *weatherReport;
	GNSSstatus_t	*gnssStatus	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} VehicleStatus_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_VehicleStatus;
extern asn_SEQUENCE_specifics_t asn_SPC_VehicleStatus_specs_1;
extern asn_TYPE_member_t asn_MBR_VehicleStatus_1[22];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "WiperSet.h"
#include "BrakeSystemStatus.h"
#include "FullPositionVector.h"
#include "SpeedandHeadingandThrottleConfidence.h"
#include "VehicleIdent.h"
#include "J1939data.h"
#include "AccelerationSet4Way.h"
#include "ConfidenceSet.h"

#endif	/* _VehicleStatus_H_ */
#include <asn_internal.h>
