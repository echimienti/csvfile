# csvfile
CSV File handling with multi dimensional vector

# UTF16 encoded csv files
On Ubuntu:
chardetect3 address.csv
address.csv: UTF-16LE with confidence 1.0
iconv -c -f UTF-16LE -t ISO-8859-1//TRANSLIT address.csv > address.txt
address.txt is the converted file which can be copied to address.csv
