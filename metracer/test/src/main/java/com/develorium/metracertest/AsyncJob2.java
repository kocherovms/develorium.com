package com.develorium.metracertest;

import com.develorium.metracer.Traced;

public class AsyncJob2 implements Runnable {
	AsyncJob2(final int theDuration) {
		duration_ = theDuration;
	}
	@Traced
	public void run() {              
		try {
			Thread.sleep(duration_);
		} catch (InterruptedException e) {
		}
	}
	private int duration_ = 0;
}
