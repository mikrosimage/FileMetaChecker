#!/usr/bin/python

import pyFileMetaChecker as qc
import argparse

parser = argparse.ArgumentParser( )
parser.add_argument( '-s', '--spec', metavar='spec', help='specification file' )
parser.add_argument( '-f', '--file', metavar='file', nargs='+', help='input file')

args = parser.parse_args()


#print args.spec
#print args.file


for f in args.file :
	report = qc.report.Report()
	qc.spec.level = qc.spec.eLogFatal

	report.setPrintVerbosity( qc.report.eReportDisplayNone );

	p = qc.processor.Processor()

	p.process( f, args.spec, report )

	listOfElements = report.get()
	#print "processing", f
	p#rint "number of elements in report", len( listOfElements )

	idMaxSize = 0
	labelMaxSize = 0

	for e in listOfElements :
		idMaxSize    = max( idMaxSize,    len( qc.report.getElem( e )._id    ) )
		labelMaxSize = max( labelMaxSize, len( qc.report.getElem( e )._label ) )

	#print "**********"
	#for e in listOfElements :
	#	#print qc.report.getElem( e )._id, qc.report.getElem( e )._label
	#	idFormatter = '{0: <' + str( idMaxSize + 1 ) + '}'
	#	labelFormatter = '{0: <' + str( labelMaxSize + 1 ) + '}'
	#	elem = qc.report.getElem( e )
	#	print idFormatter.format( elem._id ), labelFormatter.format( elem._label ), elem._dispValue
	#print "**********"

	print report.serialize( qc.report.Report.eExportTypeJson )
	#print report.serialize( qc.report.Report.eExportTypeXml )
	#print report.serialize( qc.report.Report.eExportTypeJson, True )
