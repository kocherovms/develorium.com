package com.develorium.methtracertest;

import com.develorium.methtracer.Traced;

public class App {
    public static void main( String[] args ) {
        init();
        compute();
        try {
        	computeAdditionalData();
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
	private static void computeAdditionalData() throws Exception {
		System.out.println("Computing additional data but...");
		throw new Exception("Something went wrong");
	}
    @Traced
	private static void printResults() {
		System.out.println("Result is 42");
	}
}
