Simple methods tracer for Java programs based on annotations + javaagent + javaassist.
Compile with (both agent and test program): mvn install
To run a test program: java -javaagent:${path to metracer .jar file} -jar ${path to metracertest .jar file}
