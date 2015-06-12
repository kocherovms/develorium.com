package com.develorium.methtracer;

import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.security.ProtectionDomain;

import javassist.*;

public class TracerAdder implements ClassFileTransformer {
	public byte[] transform(ClassLoader loader, String className,
				Class<?> classBeingRedefined, ProtectionDomain protectionDomain,
				byte[] classfileBuffer) throws IllegalClassFormatException {
		final String canonicalClassName = className.replaceAll("/", ".");
		ClassPool cp = ClassPool.getDefault();
		cp.insertClassPath(new ByteArrayClassPath(canonicalClassName, classfileBuffer));
		boolean wasInstrumented = false;
	
		try {
			CtClass cc = cp.get(canonicalClassName);
		
			for(CtMethod method : cc.getDeclaredMethods()) {
				Object[] annotations = null;
				try {
					annotations = method.getAnnotations();
				} catch (ClassNotFoundException e) {
					String msg = String.format("Failed to get annotations of method %1s: %2s", 
								   method.getLongName(), e.toString());
					System.err.println(msg);
					continue;
				}
			
				for (final Object ann : annotations) {
					if (ann instanceof Traced) {
						try {
							addMethodTracing(cc, method);
							wasInstrumented = true;
						} catch (Exception e) {
							String msg = String.format("Failed to add tracing to method %1s: %2s",
										   method.getLongName(), e.toString());
							System.err.println(msg);
						}
					}
				}
			}
		
			if(wasInstrumented) {
				try {
					return cc.toBytecode();
				} catch (Exception e) {
					String msg = String.format("Failed to compile instrumented class %1s: %2s",
								   canonicalClassName, e.toString());
					System.err.println(msg);
				}
			}
		} catch (NotFoundException e) {
			String msg = String.format("Failed to register class %1s in a javaassist ClassPool: %2s",
						   canonicalClassName, e.toString());
			System.err.println(msg);
		}
	
		return classfileBuffer;
	}
	private static void addMethodTracing(CtClass theClass, CtMethod theMethod) throws NotFoundException, CannotCompileException {
		String prolog = String.format("System.out.println(\"+++ %1s \");", theMethod.getLongName());
		String epilog = String.format("System.out.println(\"--- %1s \");", theMethod.getLongName());
		theMethod.insertBefore(prolog.toString());
		theMethod.insertAfter(epilog.toString(), true);
	}
}
