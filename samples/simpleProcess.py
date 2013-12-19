#!/usr/bin/python

import pyFileMetaChecker as qc
import argparse

parser = argparse.ArgumentParser( )
parser.add_argument( '-s', '--spec', metavar='spec', help='specification file' )
parser.add_argument( '-f', '--file', metavar='file', nargs='+', help='input file')

args = parser.parse_args()


print args.spec
print args.file


for f in args.file :
	report = qc.report.Report()
	qc.spec.level = qc.spec.eLogFatal

	report.setPrintVerbosity( qc.report.eReportDisplayNone );

	p = qc.processor.Processor()

	p.process( f, args.spec, report )

	listOfElements = report.get()
	print "processing", f
	print "number of elements in report", len( listOfElements )

	#for e in listOfElements :
	#	print qc.report.getElem( e )._id, qc.report.getElem( e )._label

