Import( 'project', 'libs' )


### Define global flags for the whole project
# depending on the platform and compilation mode
projectFlags = {
		'LIBPATH': [project.inOutputLib()],
		'CCFLAGS': project.CC['warning3'],
		'CXXFLAGS' : [],
		'CPPDEFINES':[
			'BOOST_ALL_DYN_LINK',
			],
	}


if project.env['mode'] == 'production' :
	# In 'production' mode set a flag FILEMETACHECKER_PRODUCTION
	projectFlags['CPPDEFINES'].append( 'FILEMETACHECKER_PRODUCTION' )
	# If your compiler as a visibility flag, hide all plugin internal things
	if 'visibilityhidden' in project.CC:
		projectFlags['SHCCFLAGS'] = [project.CC['visibilityhidden']]

# If your compiler as a flag to mark undefined flags as error in shared libraries
if 'sharedNoUndefined' in project.CC:
	projectFlags['SHLINKFLAGS'] = [project.CC['sharedNoUndefined']]

if project.env['compiler'] == "gcc" :
	projectFlags['CXXFLAGS'].append( '-std=c++11' )

# Creates a dependency target without associated code or compiled object,
# but only associated with compilation flags
fileMetaChecker = project.ObjectLibrary( 'fileMetaCheckerDefault', envFlags=projectFlags )
# Set this object library as a default library for all targets
project.commonLibs.append( fileMetaChecker )

### Load all SConscript files (in the correct order)
SConscript(
		project.scanFiles( [
				'3rdParty',
				'libraries/Common',
				'libraries/SpecReader',
				'libraries/BasicElement',
				'libraries/ElementChecker',
				'libraries/FileReader',
				'libraries/ReportGenerator',
				'libraries/Comparator',
				# 'libraries/systemInfo',
				'apps',
			], accept=['SConscript'] )
	)

