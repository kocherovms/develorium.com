/*
Copyright Michael Kocherov, 2015
http://develorium.com
*/

package com.develorium.metracertest;

import com.develorium.metracer.Traced;

public class MainJob {
	@Traced
	public void perform() {
		init("user", "123");
		compute();
		try {
			computeMoreData();
		} catch(Exception e) {
			System.err.format("Failed to compute additional data: %1$s\n", e.toString());
		}
		try {
			computeEvenMoreData();
		} catch(Exception e) {
			System.err.format("Failed to compute additional data: %1$s\n", e.toString());
		}
		computeDataLevel0();
		printResults();
	}
	@Traced
	private static void init(String theUser, String thePassword) {
		System.out.println("Initing program data");
	}
	@Traced
	private static void compute() {
		System.out.println("Computing data");
	}
	@Traced
	private static void computeMoreData() throws Exception {
		System.out.println("Computing more data but...");
		throw new Exception("Something went wrong");
	}
	@Traced
	private static void computeEvenMoreData() throws Exception {
		System.out.println("Computing even more data but...");
		try {
			throw new Exception("Something went wrong");
		} finally {
			System.err.println("Resources for even more data computation released");
		}
	}
	@Traced 
	private static void computeDataLevel0() {
		computeDataLevel1();
	}
	@Traced
	private static void computeDataLevel1() {
		computeDataLevel2();
	}
	@Traced
	private static void computeDataLevel2() {
	}
	@Traced
	private static void printResults() {
		System.out.println("Result is 42");
	}
}
