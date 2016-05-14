package com.develorium.cdi_problem.domain;

import java.io.Serializable;
import com.develorium.cdi_problem.domain.GuestMarker;

@GuestMarker
public class Guest implements Serializable {
	private static final long serialVersionUID = -424435599984364060L;
	private String name;

	public String getName() { 
		return name;
	}

	public void setName(String theName) {
		name = theName;
	}
}