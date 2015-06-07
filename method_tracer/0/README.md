Simple method tracer for Java programs based on annotations + javaagent + javaassist
Compile with (both agent and test program): mvn install
To run a test program: java -javaagent:<path to methtracer.jar> -jar methtracertest-1.0.0.jar