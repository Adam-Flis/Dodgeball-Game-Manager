import React from "react";
import { createRoot } from "react-dom/client";
import {
  createBrowserRouter,
  RouterProvider,
  Route,
  Link,
  Outlet,
} from "react-router-dom";
import Reviews from "./Routes/ShotClockPage";
import Home from "./Routes/RefPage";
import Navbar from "./Components/Navbar";
import "./App.css";

const Applayout = () => (
    <>
        <Navbar />
        <Outlet />
    </>
);

const router = createBrowserRouter([
    {
        element: <Applayout />,
        children: [
              {
                path: "/",
                element: <SettingsPage />,
              },
              {
                path: "Shot Clock Page",
                element: <ShotClockPage />,
              },
              {
                path: "Referee Page",
                element: <RefereePage />,
              },
        ],
    },
]);

createRoot(document.getElementById("root")).render(
  <RouterProvider router={router} />
);