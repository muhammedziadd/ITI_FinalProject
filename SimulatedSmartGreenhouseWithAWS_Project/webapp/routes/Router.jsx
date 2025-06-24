import { BrowserRouter, Routes, Route } from 'react-router-dom'
import Layout from '../components/Layout'
import Login from '../pages/Login'
import Dashboard from '../pages/Dashboard'
import Readings from '../pages/Readings'
import PlantHealth from '../pages/PlantHealth'
import LoadTesting from '../pages/LoadTesting'

const Router = () => {
  return (
    <BrowserRouter>
      <Routes>
        <Route path="/login" element={<Login />} />
        <Route path="/" element={<Layout />}>
          <Route index element={<Dashboard />} />
          <Route path="readings" element={<Readings />} />
          <Route path="plant-health" element={<PlantHealth />} />
          <Route path="load-testing" element={<LoadTesting />} />
        </Route>
      </Routes>
    </BrowserRouter>
  )
}

export default Router