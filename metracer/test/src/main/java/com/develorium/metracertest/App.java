package com.develorium.metracertest;

import com.develorium.metracer.Traced;

public class App {
	public static void main( String[] args ) {
		init();
		compute();
		try {
			computeMoreData();
		} catch(Exception e) {
			System.out.println("Failed to compute additional data: " + e.toString());
		}
		try {
			computeEvenMoreData();
		} catch(Exception e) {
			System.out.println("Failed to compute additional data: " + e.toString());
		}
		printResults();
	}
	@Traced
	private static void init() {
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
			System.out.println("Resources for even more data computation released");
		}
	}
	@Traced
	private static void printResults() {
		System.out.println("Result is 42");
	}
}
