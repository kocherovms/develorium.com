/*
Copyright Michael Kocherov, 2015
http://develorium.com
*/

package com.develorium.metracer;

import java.util.Stack;

public class TracingState {
	public int submerge() {
		callDepth = !frames.empty() ? frames.peek() + 1 : 0;
		frames.push(callDepth);
		return callDepth;
	}
	public void emerge() {
		if(!frames.empty()) 
				frames.pop();
	}
	public void commitEmerge() {
		if(isException()) 
			emerge();
		
		callDepth = !frames.empty() ? frames.peek() : 0;
	}
	public int getCallDepth() {
		return callDepth;
	}
	public boolean isException() {
		return !frames.empty() && frames.peek() == callDepth;
	}
	private int callDepth = 0;
	private Stack<Integer> frames = new Stack<Integer>();
}
