/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines macros for DLL exports/imports.

#ifndef ATRESTTF_EXPORT_H
#define ATRESTTF_EXPORT_H

	/// @def atresttfExport
	/// @brief Macro for DLL exports/imports.
	/// @def atresttfFnExport
	/// @brief Macro for function DLL exports/imports.
	#ifdef _LIB
		#define atresttfExport
		#define atresttfFnExport
	#else
		#ifdef _WIN32
			#ifdef ATRESTTF_EXPORTS
				#define atresttfExport __declspec(dllexport)
				#define atresttfFnExport __declspec(dllexport)
			#else
				#define atresttfExport __declspec(dllimport)
				#define atresttfFnExport __declspec(dllimport)
			#endif
		#else
			#define atresttfExport __attribute__ ((visibility("default")))
			#define atresttfFnExport __attribute__ ((visibility("default")))
		#endif
	#endif

#endif

