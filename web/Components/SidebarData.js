import React from 'react';
import * as FaIcons from "react-icons/fa";
import * as MdIcons from "react-icons/md";

export const SidebarData = [
    {
        title:"Settings Page",
        path:"/",
        icons: <FaIcons.FaHome />, //Change this icon to gear
        cName: "nav-text",
    }, 
    {
        title:"Reviews",
        path:"/Reviews",
        icons: <MdIcons.MdReviews />,
        cName: "nav-text",
    }, 
    {
        title:"Shot Clock",
        path:"/ShotClockPage",
        icons: <MdIcons.MdReviews />, //Change this icon if using icon
        cName: "nav-text",
    }, 
    {
        title:"Referee",
        path:"/RefereePage",
        icons: <MdIcons.MdReviews />, //Change this icon if using icon
        cName: "nav-text",
    }, 
]