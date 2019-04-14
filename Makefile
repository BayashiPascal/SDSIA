help: 
	python3 generateDataSet.py -help

cleanDataSet:
	rm -r DataSets/*

generate:
	python3 generateDataSet.py

regenerate: 
	python3 generateDataSet.py -force

simul: 
	python3 generateDataSet.py -simul

listing: 
	python3 generateDataSet.py -list

unitTest:
	python3 generateDataSet.py -unitTest

exampleUse:
	python3 exampleUse.py UnitTestOut/001/001/

