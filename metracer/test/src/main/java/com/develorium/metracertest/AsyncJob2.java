/*
Copyright Michael Kocherov, 2015
http://develorium.com
*/

package com.develorium.metracertest;

import com.develorium.metracer.Traced;

public class AsyncJob2 implements Runnable {
	AsyncJob2(final int theDuration) {
		duration_ = theDuration;
	}
	@Traced
	public void run() {
		perform();
	}
	@Traced
	public void perform() {
		try {
			Thread.sleep(duration_);
		} catch (InterruptedException e) {
		}
	}
	private int duration_ = 0;
}
