/*
Copyright Michael Kocherov, 2015
http://develorium.com
*/

package com.develorium.metracertest;

public class Unannotated {
	public static void main( String[] args ) {
		testA();
		int res = testB(false, true);
		System.out.println("result of testB = " + res);
		testC(42, 2.718f, 3.1415);
	}
	static void testA() {
		System.out.println("testA");
	}
	static int testB(boolean theBool1, boolean theBool2) {
		return 42;
	}
	static void testC(int theInt, float theFloat, double theDouble) {
	}
}