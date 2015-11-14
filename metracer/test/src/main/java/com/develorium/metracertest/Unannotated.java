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
				testInheritanceBackward();
				testInheritanceForward();
		}
		static void testA() {
				System.out.println("testA");
		}
		static int testB(boolean theBool1, boolean theBool2) {
				if(!theBool1)
						return testB(true, true);

				return 42;
		}
		static void testC(int theInt, float theFloat, double theDouble) {
				testInnerC();
		}
		static void testInnerC() {
				try {
						testFaulty();
				} catch(Throwable e) {
				}
		}
		static void testFaulty() throws Throwable {
				throw new Throwable();
		}
		static class InnerBase {
				public void test() {
						System.out.println("InnerBase.test");
				}
				public void test2() {	
						System.out.println("InnerBase.test2");
				}
		}
		static class InnerChild extends InnerBase {
				public void test() {
						System.out.println("InnerChild.test");
						super.test();
				}
				public void test2() {	
						System.out.println("InnerChild.test2");
				}
		}
		static void testInheritanceBackward() {
				InnerChild i = new InnerChild();
				i.test();
		}
		static void testInheritanceForward() {
				InnerBase i = new InnerChild();
				i.test2();
		}
}