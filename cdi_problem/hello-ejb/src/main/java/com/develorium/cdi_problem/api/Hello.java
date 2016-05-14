package com.develorium.cdi_problem.api;

import javax.ejb.Local;

@Local
public interface Hello {
    public String getHelloText();
}