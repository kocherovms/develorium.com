package com.develorium.cdi_problem.ejb;

import com.develorium.cdi_problem.Guest;
import com.develorium.cdi_problem.GuestMarker;
import com.develorium.cdi_problem.api.Hello;
import javax.ejb.Stateless;
import javax.inject.Inject;
import javax.enterprise.context.RequestScoped;
import javax.enterprise.inject.spi.CDI;

@Stateless
public class HelloBean implements Hello {
	@Inject
	@RequestScoped
	@GuestMarker
	private Guest guest;

	@Override
	public String getHelloText() {
		//Guest guest = CDI.current().select(Guest.class).get();
		return String.format("Hello, %s (%s)!\n", guest != null ? guest.getName() : null, guest);
	}
}