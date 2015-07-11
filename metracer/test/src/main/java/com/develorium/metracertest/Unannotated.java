/*
Copyright Michael Kocherov, 2015
http://develorium.com
*/

package com.develorium.metracertest;

public class Unannotated {
	public static void main( String[] args ) {
		testDummy();
		testBool(false, true);
		testNumbers(42, 2.718f, 3.1415);
		testBoxedNumbers(42, 2.718f, 3.1415);
		testString("Hello, world!");
		testLongString(new String(new char[100]).replace("\0", "Hello, world!"));
		testStructure(new Structure());
		new Unannotated().testNonStaticMethod("Hello, world!", null);
	}
	static void testDummy() {
	}
	static void testBool(boolean theBool1, boolean theBool2) {
	}
	static void testNumbers(int theInt, float theFloat, double theDouble) {
	}
	static void testBoxedNumbers(Integer theInt, Float theFloat, Double theDouble) {
	}
	static void testString(String theString) {
	}
	static void testLongString(String theLongString) {
	}
	static void testStructure(Structure theStructure) {
	}
	void testNonStaticMethod(String theString, Object theObject) {
	}
	static class Structure {
	}
}