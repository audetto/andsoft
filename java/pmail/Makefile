all: clean
	gawk -f version.awk Version.java > Version.temp && cp Version.temp Version.java
	javac ReaderX.java

clean:
	rm -f *.class

debug: clean
	gawk -f version.awk Version.java > Version.temp && cp Version.temp Version.java
	javac -g -deprecation ReaderX.java
