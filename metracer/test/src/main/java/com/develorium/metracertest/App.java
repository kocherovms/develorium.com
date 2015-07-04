/*
Copyright Michael Kocherov, 2015
http://develorium.com
*/

package com.develorium.metracertest;

import java.util.Random;
import com.develorium.metracer.Traced;

public class App {
	@Traced
	public static void main( String[] args ) {
		Thread[] jobs = { new Thread(new AsyncJob1(1 + random.nextInt(100))), new Thread(new AsyncJob2(1 + random.nextInt(100))) };
		
		for(Thread job : jobs) 
			job.start();
					
		MainJob mainJob = new MainJob();
		mainJob.perform();
		
		for(Thread job : jobs) {
			try {
				job.join();
			} catch (InterruptedException e) {
			}
		}
	}
	private final static Random random = new Random();
}
