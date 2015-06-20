package com.develorium.metracer;

public class TracingStateThreadLocal extends ThreadLocal<TracingState> {
	@Override 
	protected TracingState initialValue() {
		return new TracingState();
	}
	public static final TracingStateThreadLocal instance = new TracingStateThreadLocal();
}
