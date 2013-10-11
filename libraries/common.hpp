#ifndef _QICHE_COMMON_HPP_
#define _QICHE_COMMON_HPP_

enum EType
{
	eTypeData = 0,
	eTypeNumber
};

enum EStatus
{
	eStatusNotCheck = 0,
	eStatusValid,
	eStatusInvalid,
	eStatusInvalidButOptional,
	eStatusInvalidForUnordered,
	eStatusInvalidButSkip,
	eStatusPassOverData,
	eStatusSkip
};

#endif
