import os
from subprocess import *

def fileAnalyse( inputFile ):
	# print "*** CHECK ! ***"
	# print inputFile

	cmdQc = 'mikqc "' + inputFile + '"'
	processQc = Popen( cmdQc, stdout=PIPE, stderr=PIPE, env=os.environ, shell=True )
	mikqcReport = inputFile + ".xml"

	# print "*** DONE ! (CHECK) ***"
	return mikqcReport


def loudnessAnalyse( inputFile, loudnessStandard=["ebu"] ):
	# print "*** LOUDNESS ! ***"
	# print inputFile
	# print loudnessStandard

	loudness = ""
	for standard in loudnessStandard :
		loudness += " --standard=" + standard + " "
	loudnessCmd = os.environ['LOUDNESS_PATH'] + " "  + loudness + inputFile
	call( loudnessCmd, shell=True )

	loudnessReport = inputFile.replace( ".wav", "_PLoud.xml" )

	# print "*** DONE ! (LOUDNESS) ***"
	return loudnessReport