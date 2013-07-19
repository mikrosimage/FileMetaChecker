Import( 'project', 'libs' )


### Define global flags for the whole project
# depending on the platform and compilation mode
qualityCheckFlags = {
		'LIBPATH': [project.inOutputLib()],
		'CCFLAGS': project.CC['warning3'],
		'CPPDEFINES':[
			'BOOST_ALL_DYN_LINK',
			],
	}

if project.env['mode'] == 'production' :
	# In 'production' mode set a flag QUALITYCHECK_PRODUCTION
	qualityCheckFlags['CPPDEFINES'].append( 'QUALITYCHECK_PRODUCTION' )
	# If your compiler as a visibility flag, hide all plugin internal things
	if 'visibilityhidden' in project.CC:
		qualityCheckFlags['SHCCFLAGS'] = [project.CC['visibilityhidden']]

# If your compiler as a flag to mark undefined flags as error in shared libraries
if 'sharedNoUndefined' in project.CC:
	qualityCheckFlags['SHLINKFLAGS'] = [project.CC['sharedNoUndefined']]

# Creates a dependency target without associated code or compiled object,
# but only associated with compilation flags
qualityCheck = project.ObjectLibrary( 'qualityCheckDefault', envFlags=qualityCheckFlags )
# Set this object library as a default library for all targets
project.commonLibs.append( qualityCheck )



### Load all SConscript files (in the correct order)
SConscript(
		project.scanFiles( [
				'libraries/common',
				'libraries/basicElement',
				'libraries/fileReader',
				'libraries/specReader',
				'libraries/report',
				'libraries/systemInfo',
				'libraries/extractor',
				'libraries/analyser',
				'apps',
			], accept=['SConscript'] )
	)


