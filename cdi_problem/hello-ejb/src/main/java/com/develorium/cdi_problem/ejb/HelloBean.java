package com.develorium.cdi_problem.ejb;

import com.develorium.cdi_problem.api.Hello;
import com.develorium.cdi_problem.domain.Guest;
import com.develorium.cdi_problem.domain.GuestMarker;
import javax.ejb.Stateless;
import javax.inject.Inject;

@Stateless
public class HelloBean implements Hello {
	@Inject
	@GuestMarker
	Guest guest;

	@Override
	public String getHelloText() {
		return String.format("Hello, %s!\n", guest.getName());
	}
}