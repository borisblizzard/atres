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

#ifndef ATRES_EXPORT_H
#define ATRES_EXPORT_H

	/// @def atresExport
	/// @brief Macro for DLL exports/imports.
	/// @def atresFnExport
	/// @brief Macro for function DLL exports/imports.
	#ifdef _LIB
		#define atresExport
		#define atresFnExport
	#else
		#ifdef _WIN32
			#ifdef ATRES_EXPORTS
				#define atresExport __declspec(dllexport)
				#define atresFnExport __declspec(dllexport)
			#else
				#define atresExport __declspec(dllimport)
				#define atresFnExport __declspec(dllimport)
			#endif
		#else
			#define atresExport __attribute__ ((visibility("default")))
			#define atresFnExport __attribute__ ((visibility("default")))
		#endif
	#endif

#endif

