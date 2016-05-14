package com.develorium.cdi_problem.rest;

import com.develorium.cdi_problem.api.Hello;
import com.develorium.cdi_problem.domain.Guest;
import com.develorium.cdi_problem.domain.GuestMarker;
import javax.ejb.EJB;
import javax.enterprise.context.RequestScoped;
import javax.enterprise.inject.Produces;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

@Path("/hello")
public class HelloService {
	@EJB
	private Hello hello;

	@Produces
	@RequestScoped
	@GuestMarker
	private Guest guest;

	@GET
	public Response perform() {
		guest = new Guest();
		guest.setName("white rabbit");
		String text = hello.getHelloText();
		return Response.ok().type(MediaType.TEXT_PLAIN).entity(text).build();
	}
}