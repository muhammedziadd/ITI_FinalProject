import { Link, Outlet, useNavigate } from 'react-router-dom'

const Layout = () => {
  const navigate = useNavigate()

  const handleLogout = () => {
    // Add your logout logic here
    navigate('/login')
  }

  return (
    <div className="min-h-screen bg-gray-50">
      <nav className="bg-[#30825C] text-white p-4">
        <div className="container mx-auto flex justify-between items-center">
          <div className="flex items-center">
            <img src="/assets/images/logoI.svg" alt="Logo" className="h-12 w-auto mr-2" />
          </div>
          <div className="space-x-4">
            <Link to="/" className="hover:text-green-200">Dashboard</Link>
            <Link to="/readings" className="hover:text-green-200">Readings</Link>
            <Link to="/plant-health" className="hover:text-green-200">Plant Health</Link>
            <Link to="/load-testing" className="hover:text-green-200">Load Testing</Link>
            <button
              onClick={handleLogout}
              className="ml-4 px-4 py-2 bg-[#145B38] rounded hover:bg-[#2DA066]"
            >
              Logout
            </button>
          </div>
        </div>
      </nav>
      <main className="container mx-auto p-4">
        <Outlet />
      </main>
    </div>
  )
}

export default Layout