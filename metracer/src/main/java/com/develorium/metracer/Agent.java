/*
Copyright Michael Kocherov, 2015
http://develorium.com
*/

package com.develorium.metracer;

import java.lang.instrument.Instrumentation;

public class Agent {
	public static void premain(String args, Instrumentation instrumentation) {
		instrumentation.addTransformer(new TracerAdder(args));
	}
}
