/*
Copyright Michael Kocherov, 2015
http://develorium.com
*/

package com.develorium.metracer;

import java.util.Stack;

public class TracingState {
	public int submerge() {
		frames.push(false);
		return frames.size() - 1;
	}
	public void emerge() {
		if(!frames.empty()) { 
			frames.pop();
			frames.push(true); // means this frame is emerged
		}
	}
	public void commitEmerge() {
		if(!frames.empty())
			frames.pop();
	}
	public int getCallDepth() {
		return frames.size() - 1;
	}
	public boolean isException() {
		return !frames.empty() && frames.peek() == false;
	}
	private Stack<Boolean> frames = new Stack<Boolean>();
}
