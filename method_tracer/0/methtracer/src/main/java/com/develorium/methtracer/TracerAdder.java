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
					System.err.println("Failed to get annotations of method " + method.getLongName() + ": " + e.toString());
					continue;
				}
				
				for (final Object ann : annotations) {
					if (ann instanceof Traced) {
						try {
							addMethodTracing(cc, method);
							wasInstrumented = true;
						} catch (Exception e) {
							System.err.println("Failed to add tracing to method " + method.getLongName() + ": " + e.toString());
						}
					}
				}
			}
			
			if(wasInstrumented) {
				try {
					return cc.toBytecode();
				} catch (Exception e) {
					System.err.println("Failed to compile instrumented class " + canonicalClassName + ": " + e.toString());
					return classfileBuffer;
				}
			}
		} catch (NotFoundException e) {
			System.err.println("Failed to register class " + canonicalClassName + " in a javaassist ClassPool: " + e.toString());
			return classfileBuffer;
		}
		
		return classfileBuffer;
	}
	private static void addMethodTracing(CtClass theClass, CtMethod theMethod) throws NotFoundException, CannotCompileException {
		StringBuffer prolog = new StringBuffer();
		prolog.append("System.out.println(\"+++ " + theMethod.getLongName() + "\");");
		StringBuffer epilog = new StringBuffer();
		epilog.append("System.out.println(\"--- " + theMethod.getLongName() + "\");");
		
		theMethod.insertBefore(prolog.toString());
		theMethod.insertAfter(epilog.toString(), true);
	}
}
