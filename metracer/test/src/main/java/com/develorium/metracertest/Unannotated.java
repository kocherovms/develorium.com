/*
Copyright Michael Kocherov, 2015
http://develorium.com
*/

package com.develorium.metracertest;

public class Unannotated {
	public static void main( String[] args ) {
		testA();
		testB(false, true);
		testC(42, 2.718f, 3.1415);
	}
	static void testA() {
	}
	static void testB(boolean theBool1, boolean theBool2) {
	}
	static void testC(int theInt, float theFloat, double theDouble) {
	}
}