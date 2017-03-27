package com.develorium.cdi_problem.rest;

import com.develorium.cdi_problem.api.Hello;
//import com.develorium.cdi_problem.domain.Guest;
import com.develorium.cdi_problem.Guest;
import com.develorium.cdi_problem.GuestMarker;
import javax.ejb.EJB;
import javax.enterprise.context.RequestScoped;
import javax.enterprise.inject.Produces;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.inject.Inject;
import javax.enterprise.inject.spi.CDI;

@Path("/hello")
public class HelloService {

	//@Produces
	////@Inject
	//@RequestScoped
	////@GuestMarker
	//private Guest guest;

	@EJB
	private Hello hello;

	@Produces
	@RequestScoped
	@GuestMarker
	private Guest generateGuest() {
		System.out.println("kms@");
		Guest g = new Guest();
		g.setName("white rabbit");
		return g;
	}

	@GET
	public Response perform(@Inject Guest theGuest) {
		//Guest guest = CDI.current().select(Guest.class).get();
		//guest = new Guest();
		//guest.setName("white rabbit");
		String text = hello.getHelloText(); // + "\n" + guest;
		return Response.ok().type(MediaType.TEXT_PLAIN).entity(text).build();
	}
}