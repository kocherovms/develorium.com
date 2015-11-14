/*
Copyright Michael Kocherov, 2015
http://develorium.com
*/

package com.develorium.metracer;

import java.util.*;
import javassist.*;

class ClassPools {
		private Map<Integer, ClassPool> classPools = new HashMap<Integer, ClassPool>();
		
		public ClassPool getClassPool(ClassLoader theLoader) {
				if(theLoader == null) 
						return ClassPool.getDefault();

				Integer loaderHashCode = theLoader.hashCode();

				if(classPools.containsKey(loaderHashCode))
						return classPools.get(loaderHashCode);

				ClassPool parentClassPool = getClassPool(theLoader.getParent());
				ClassPool newClassPool = new ClassPool(parentClassPool);
				newClassPool.appendSystemPath();
				LoaderClassPath loaderClassPath = new LoaderClassPath(theLoader);
				newClassPool.appendClassPath(loaderClassPath);
				classPools.put(loaderHashCode, newClassPool);
				return newClassPool;
		}
}