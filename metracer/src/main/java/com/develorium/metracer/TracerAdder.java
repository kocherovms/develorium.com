package com.develorium.metracer;

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
					System.err.format("Failed to get annotations of method %1$s: %2$s\n", 
							method.getLongName(), e.toString());
					continue;
				}
			
				for (final Object ann : annotations) {
					if (ann instanceof Traced) {
						try {
							addMethodTracing(cc, method);
							wasInstrumented = true;
						} catch (Exception e) {
							System.err.format("Failed to add tracing to method %1$s: %2$s\n", 
									method.getLongName(), e.toString());
						}
					}
				}
			}
		
			if(wasInstrumented) {
				try {
					return cc.toBytecode();
				} catch (Exception e) {
					System.err.format("Failed to compile instrumented class %1$s: %2$s\n",
							canonicalClassName, e.toString());
				}
			}
		} catch (NotFoundException e) {
			System.err.format("Failed to register class %1$s in a javaassist ClassPool: %2$s\n",
					   canonicalClassName, e.toString());
		}
	
		return classfileBuffer;
	}
	private static void addMethodTracing(CtClass theClass, CtMethod theMethod) throws NotFoundException, CannotCompileException {
		final String VarType = "com.develorium.metracer.TracingState";
		final String VarName = "__com_develorium_tracing_state";
		String prolog = 
					String.format("%1$s %2$s = (%1$s)%3$s.get();", VarType, VarName, TracingStateFieldName) +
					String.format("System.out.println(\"+++[\" + %1$s.submerge() + \"] %2$s\");", VarName, theMethod.getLongName());
		String epilog = 
					String.format("%1$s %2$s = (%1$s)%3$s.get();", VarType, VarName, TracingStateFieldName) +
					String.format("System.out.println(\"---[\" + %1$s.getCallDepth() + \"] %2$s\");", VarName, theMethod.getLongName()) +
					String.format("%1$s.emerge();", VarName);
		String epilogFinally = 
				String.format("%1$s %2$s = (%1$s)%3$s.get();", VarType, VarName, TracingStateFieldName) +
				String.format("if(%1$s.isException()) { System.out.println(\"---[\" + %1$s.getCallDepth() + \"] %2$s (by exception)\"); }", 
							VarName, theMethod.getLongName()) +
				String.format("%1$s.commitEmerge();", VarName);
		theMethod.insertBefore(prolog.toString());
		theMethod.insertAfter(epilog.toString());
		theMethod.insertAfter(epilogFinally.toString(), true);
	}
	private static final String TracingStateFieldName = "com.develorium.metracer.TracingStateThreadLocal.instance";
}
