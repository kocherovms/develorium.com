/*
Copyright Michael Kocherov, 2015
http://develorium.com
*/

package com.develorium.metracer;

import java.lang.instrument.*;
import java.security.ProtectionDomain;
import java.util.*;
import java.util.regex.*;

import javassist.*;
import javassist.bytecode.*;

public class TracerAdder implements ClassFileTransformer {
	private String selfPackageName = null;
	private Pattern pattern = null;
	private Set<Integer> touchedClassLoaders = new HashSet<Integer>();

	public TracerAdder(String theArguments) {
		selfPackageName = this.getClass().getPackage().getName() + ".";

		if(theArguments == null)
			return;

		final String PatternStanza = "pattern=";

		if(theArguments.startsWith(PatternStanza)) {
			String patternString = theArguments.substring(PatternStanza.length());
			try {
				pattern = Pattern.compile(patternString);
			} catch(PatternSyntaxException e) {
				System.err.format("Provided pattern \"%1$s\" is malformed: %2$s\n", patternString, e.toString());
			}
		}
	}
	public byte[] transform(ClassLoader loader, String className,
				Class<?> classBeingRedefined, ProtectionDomain protectionDomain,
				byte[] classfileBuffer) throws IllegalClassFormatException {
		final String canonicalClassName = className.replaceAll("/", ".");

		if(canonicalClassName.indexOf("java.lang") == 0)
			return classfileBuffer;

		ClassPool cp = ClassPool.getDefault();
		cp.insertClassPath(new ByteArrayClassPath(canonicalClassName, classfileBuffer));
		Integer loaderHashCode = loader.hashCode();

		if(!touchedClassLoaders.contains(loaderHashCode)) {
				touchedClassLoaders.add(loaderHashCode);
				System.out.println("kms@ Touched class loader " + loader.toString());
				LoaderClassPath loaderClassPath = new LoaderClassPath(loader);
				cp.appendClassPath(loaderClassPath);
				//System.out.println(loaderClassPath.toString());
		}
	
		try {
			CtClass cc = cp.get(canonicalClassName);

			if(cc.isFrozen()) {
				return classfileBuffer;
			}

			boolean wasInstrumented = false; //instrumentViaAnnotation(cc);

			if(!wasInstrumented && pattern != null)
				wasInstrumented = instrumentViaPattern(cc);
		
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
	private boolean instrumentViaAnnotation(CtClass theClass) {
		boolean wasInstrumented = false;

		for(CtMethod method : theClass.getDeclaredMethods()) {
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
						if(addMethodTracing(theClass, method))
							wasInstrumented = true;
					} catch (Exception e) {
						System.err.format("Failed to add tracing to method %1$s: %2$s\n", 
								  method.getLongName(), e.toString());
					}
				}
			}
		}

		return wasInstrumented;
	}
	private boolean instrumentViaPattern(CtClass theClass) {
		boolean wasInstrumented = false;

		for(CtMethod method : theClass.getDeclaredMethods()) {
			try {
				final String methodNameForPatternMatching = String.format("%1$s::%2$s", theClass.getName(), method.getName());
				
				if(pattern.matcher(methodNameForPatternMatching).find(0)) {
					if(addMethodTracing(theClass, method))
						wasInstrumented = true;
				}
			} catch (Exception e) {
				System.err.format("Failed to add tracing to method %1$s: %2$s\n", 
						  method.getLongName(), e.toString());
			}
		}

		return wasInstrumented;
	}
	private boolean isMethodInstrumentable(CtMethod theMethod) {
		// Do not try to patch ourselves
		if(theMethod.getLongName().startsWith(selfPackageName))
			return false;
		// Do not try to patch interface / abstract / native methods
		int modifiers = theMethod.getModifiers();
		return !Modifier.isInterface(modifiers) && !Modifier.isAbstract(modifiers) && !Modifier.isNative(modifiers);
	}
	private String getArgumentPrintingCode(CtMethod theMethod, String theResultHolderName) {
		StringBuilder rv = new StringBuilder();
		// Generated code tries to avoid unnecessary objects' creation. 
		// Also we don't use generics due to javaassist limitations
		rv.append("java.lang.StringBuilder resultHolderName = null;");
		rv.append("java.util.HashMap argumentNames = null;");
		rv.append("if($args.length > 0) {");
		rv.append(" resultHolderName = new java.lang.StringBuilder();");
		rv.append(" argumentNames = new java.util.HashMap();");

		MethodInfo methodInfo = theMethod.getMethodInfo();
		
		if(methodInfo != null && methodInfo.getCodeAttribute() != null) {
			LocalVariableAttribute table = (LocalVariableAttribute)methodInfo.getCodeAttribute().getAttribute(LocalVariableAttribute.tag);

			if(table != null) {
				// this is to skip a 'this' argument in case of a non-static method
				int offset = Modifier.isStatic(theMethod.getModifiers()) ? 0 : 1;
		
				for(int i = 0; i < table.tableLength(); ++i) {
					int argumentIndex = table.index(i) - offset;
		
					if(argumentIndex < 0) continue;
				
					String argumentName = table.variableName(i);
					rv.append(" argumentNames.put(new Integer(");
					rv.append(argumentIndex);
					rv.append("), \"");
					rv.append(argumentName);
					rv.append("\");");
				}
			}
		}

		rv.append("}");
		rv.append("for(int i = 0; i < $args.length; ++i) {");
		rv.append(" java.lang.Object rawN = argumentNames.get(new Integer(i));");
		rv.append(" java.lang.String n = rawN == null ? \"<unk>\" : (String)rawN;");
		rv.append(" java.lang.Object o = $args[i];");
		rv.append(" java.lang.String v = o == null ? \"null\" : o.toString();");
		rv.append(" if(v.length() > 128) v = v.substring(0, 128) + \"...\";");
		rv.append(" if(i > 0) resultHolderName.append(\", \");");
		rv.append(" resultHolderName.append(n);");
		rv.append(" resultHolderName.append(\" = \");");
		rv.append(" resultHolderName.append(v);");
		rv.append("}");
		return rv.toString().replaceAll("resultHolderName", theResultHolderName);
	}
	private boolean addMethodTracing(CtClass theClass, CtMethod theMethod) throws NotFoundException, CannotCompileException {
		if(!isMethodInstrumentable(theMethod))
			return false;

		String methodNameForPrinting = String.format("%s.%s", theClass.getName(), theMethod.getName());
		String originalMethodName = theMethod.getName();
		String wrappedMethodName = originalMethodName + MetracedSuffix;
		CtMethod wrappedMethod = CtNewMethod.copy(theMethod, wrappedMethodName, theClass, null);
		theClass.addMethod(wrappedMethod);
		
		StringBuilder body = new StringBuilder();
		body.append("{");
		body.append("java.lang.Thread thread = java.lang.Thread.currentThread();");
		body.append("java.lang.StackTraceElement[] stackTraceElements = thread.getStackTrace();");
		body.append("int callDepth = 0;");
		body.append("for(int i = 0; i < stackTraceElements.length; i++) {");
		body.append(" java.lang.StackTraceElement element = stackTraceElements[i];");
		body.append(String.format(" if(element.getMethodName().indexOf(\"%1$s\") >= 0) {", MetracedSuffix));
		body.append("  callDepth++;");
		body.append(" }");
		body.append("}");
		body.append(getArgumentPrintingCode(theMethod, "argumentValuesRaw"));
		body.append("java.lang.String argumentValues = argumentValuesRaw == null ? \"\" : argumentValuesRaw.toString();");
		body.append(String.format("System.out.println(\"+++[\" + callDepth + \"] %1$s(\" + argumentValues + \")\");", methodNameForPrinting));
		body.append("boolean isFinishedOk = false;");
		body.append("try {");

		CtClass returnType = theMethod.getReturnType();
		String callMethodCode = "";
		String returnResultCode = "";

		if(returnType == CtClass.voidType) {
			callMethodCode = String.format("%1$s($$);", wrappedMethodName);
		}
		else {
			callMethodCode = String.format("%1$s rv = %2$s($$);", returnType.getName(), wrappedMethodName);
			returnResultCode = "return rv;";
		}

		body.append(callMethodCode);
		body.append("isFinishedOk = true;");
		body.append(returnResultCode);

		body.append("} finally {");
		body.append("java.lang.String trailingExceptionInfo = isFinishedOk ? \"\" : \" (by exception)\";");
		body.append(String.format("System.out.println(\"---[\" + callDepth + \"] %1$s\" + trailingExceptionInfo);", methodNameForPrinting));
		body.append("}");
		body.append("}");
		theMethod.setBody(body.toString());
		return true;
	}
	static final String MetracedSuffix = "_com_develorium_metraced";
}
