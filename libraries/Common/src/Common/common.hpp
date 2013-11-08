#ifndef _COMMON_COMMON_HPP_
#define _COMMON_COMMON_HPP_

#include <string>
#include <map>

// standard definition labels
static const std::string kExtension = "extension";
static const std::string kFooter    = "footer";
static const std::string kHeader    = "header";
static const std::string kStandard  = "standard";

// node labels (left)
static const std::string kId     = "id";
static const std::string kLabel  = "label";

static const std::string kType   = "type";
static const std::string kCount  = "count";
static const std::string kValues = "values";
static const std::string kMap    = "map";
static const std::string kRange  = "range";
static const std::string kMin    = "min";
static const std::string kMax    = "max";


static const std::string kGroup         = "group";
static const std::string kGroupSize     = "groupSize";
static const std::string kRepetition    = "repeated";
static const std::string kRequired      = "required";
static const std::string kEndian        = "endian";
static const std::string kOptional      = "optional";
static const std::string kOrdered       = "ordered";
static const std::string kDisplayType   = "displayType";

// node values (right)
static const std::string kTrue   = "true";
static const std::string kFalse  = "false";
static const std::string kEndianBig     = "big";
static const std::string kEndianLittle  = "little";

static const std::string kAscii  = "ascii";
static const std::string kHexa   = "hexa";
static const std::string kRaw    = "raw";

static const std::string kInt8    =  "int8";
static const std::string kUInt8   = "uint8";
static const std::string kInt16   =  "int16";
static const std::string kUInt16  = "uint16";
static const std::string kInt32   =  "int32";
static const std::string kUInt32  = "uint32";
static const std::string kInt64   =  "int64";
static const std::string kUInt64  = "uint64";

static const std::string kFloat   =  "float";
static const std::string kDouble  = "double";

static const std::string kIeeeExtended  = "ieeeExtended";

static const std::string kExif = "exif";
static const std::string kKlv  = "klv";

// report definitions

static const std::string kValidator      = "fileValidator";
static const std::string kSpecification  = "specification";
static const std::string kElement        = "element";

static const std::string kValue  = "value";
static const std::string kData   = "data";
static const std::string kStatus = "status";

static const std::string kUnknown  = "unknown";
static const std::string kValid    = "valid";
static const std::string kInvalid  = "invalid";
static const std::string kPassOver = "pass over";

static const std::string kError    = "error";
static const std::string kWarning  = "warning";

enum EType
{
	eTypeUnknown = 0,
	eTypeInt8,
	eTypeUInt8,
	eTypeInt16,
	eTypeUInt16,
	eTypeInt32,
	eTypeUInt32,
	eTypeInt64,
	eTypeUInt64,
	eTypeFloat,
	eTypeDouble,
	eTypeIeeeExtended,
	eTypeAscii,
	eTypeHexa,
	eTypeRaw,
	eTypeExif,
	eTypeKlv,
};

enum EDisplayType
{
	eDisplayTypeDefault = 0,
	eDisplayTypeNumbers,
	eDisplayTypeAscii,
	eDisplayTypeHexa,
	eDisplayTypeRaw,
};

enum EStatus
{
	eStatusNotChecked = 0,
	eStatusValid,
	eStatusInvalid,
	eStatusInvalidButOptional,
	eStatusInvalidForUnordered,
	eStatusInvalidButSkip,
	eStatusInvalidGroupForIteration,
	eStatusPassOver,
	eStatusSkip,
};

static const std::map< std::string, EType > typeMap
{
	{ kInt8,         eTypeInt8         },
	{ kUInt8,        eTypeUInt8        },
	{ kInt16,        eTypeInt16        },
	{ kUInt16,       eTypeUInt16       },
	{ kInt32,        eTypeInt32        },
	{ kUInt32,       eTypeUInt32       },
	{ kInt64,        eTypeInt64        },
	{ kUInt64,       eTypeUInt64       },
	{ kFloat,        eTypeFloat        },
	{ kDouble,       eTypeDouble       },
	{ kIeeeExtended, eTypeIeeeExtended },
	{ kAscii,        eTypeAscii        },
	{ kHexa,         eTypeHexa         },
	{ kRaw,          eTypeRaw          },
	{ kExif,         eTypeExif         },
	{ kKlv,          eTypeKlv          },
};

static const std::map< std::string, EDisplayType > displayTypeMap
{
	{ "",            eDisplayTypeDefault },
	{ kInt8,         eDisplayTypeNumbers },
	{ kUInt8,        eDisplayTypeNumbers },
	{ kInt16,        eDisplayTypeNumbers },
	{ kUInt16,       eDisplayTypeNumbers },
	{ kInt32,        eDisplayTypeNumbers },
	{ kUInt32,       eDisplayTypeNumbers },
	{ kInt64,        eDisplayTypeNumbers },
	{ kUInt64,       eDisplayTypeNumbers },
	{ kFloat,        eDisplayTypeNumbers },
	{ kDouble,       eDisplayTypeNumbers },
	{ kIeeeExtended, eDisplayTypeNumbers },
	{ kAscii,        eDisplayTypeAscii   },
	{ kHexa,         eDisplayTypeHexa    },
	{ kRaw,          eDisplayTypeRaw     },
};

static const std::map< EStatus, std::string > statusMap
{
	{ eStatusNotChecked,               "Not checked"                 },
	{ eStatusValid,                    "Valid"                       },
	{ eStatusInvalid,                  "Invalid"                     },
	{ eStatusInvalidButOptional,       "Invalid but optional"        },
	{ eStatusInvalidForUnordered,      "Invalid for unordered"       },
	{ eStatusInvalidButSkip,           "Invalid but skip"            },
	{ eStatusInvalidGroupForIteration, "Invalid group for iteration" },
	{ eStatusPassOver,                 "Pass over"                   },
	{ eStatusSkip,                     "Skip"                        },
};

#endif
