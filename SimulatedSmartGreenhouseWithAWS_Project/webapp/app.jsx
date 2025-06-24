// SmartGreenhouseDashboard.jsx
import React, { useState, useEffect } from "react";
import axios from "axios";
import { LineChart, Line, XAxis, YAxis, Tooltip, Legend, ResponsiveContainer } from "recharts";

const Dashboard = () => {
  const [data, setData] = useState([]);
  const [auth, setAuth] = useState(false);
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");

  useEffect(() => {
    if (auth) {
      fetchData();
    }
  }, [auth]);

  const fetchData = async () => {
    try {
      const res = await axios.get("https://your-api-endpoint.amazonaws.com/sensors");
      setData(res.data);
    } catch (error) {
      console.error("Error fetching data:", error);
    }
  };

  const handleLogin = () => {
    if (username === "admin" && password === "admin") {
      setAuth(true);
    } else {
      alert("Invalid credentials");
    }
  };

  if (!auth) {
    return (
      <div className="flex flex-col items-center justify-center min-h-screen bg-gray-100">
        <div className="bg-white p-8 rounded shadow-md w-full max-w-sm">
          <h2 className="text-2xl font-bold mb-4 text-center">Smart Greenhouse Login</h2>
          <input
            type="text"
            placeholder="Username"
            className="w-full p-2 mb-4 border border-gray-300 rounded"
            value={username}
            onChange={(e) => setUsername(e.target.value)}
          />
          <input
            type="password"
            placeholder="Password"
            className="w-full p-2 mb-4 border border-gray-300 rounded"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
          />
          <button
            className="w-full bg-green-600 text-white p-2 rounded hover:bg-green-700"
            onClick={handleLogin}
          >
            Login
          </button>
        </div>
      </div>
    );
  }

  return (
    <div className="p-4 bg-gray-50 min-h-screen">
      <h1 className="text-3xl font-bold mb-6 text-center">Smart Greenhouse Dashboard</h1>
      <ResponsiveContainer width="100%" height={400}>
        <LineChart data={data}>
          <XAxis dataKey="time" />
          <YAxis />
          <Tooltip />
          <Legend />
          <Line type="monotone" dataKey="temperature" stroke="#ff7300" />
          <Line type="monotone" dataKey="humidity" stroke="#387908" />
          <Line type="monotone" dataKey="soil_moisture" stroke="#00bcd4" />
          <Line type="monotone" dataKey="light" stroke="#fbc02d" />
        </LineChart>
      </ResponsiveContainer>
    </div>
  );
};

export default Dashboard;
