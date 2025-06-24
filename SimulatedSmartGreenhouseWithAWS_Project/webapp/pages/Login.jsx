import React, { useState } from 'react'
import { useNavigate } from 'react-router-dom'

const Login = () => {
  const [credentials, setCredentials] = useState({ username: '', password: '' })
  const navigate = useNavigate()

  const handleSubmit = (e) => {
    e.preventDefault()
    if (credentials.username === 'admin' && credentials.password === 'admin') {
      navigate('/')
    } else {
      setCredentials({ username: '', password: '' })
      
      // Add shake animation to form
      const formElement = document.querySelector('form')
      formElement.classList.add('animate-shake')
      setTimeout(() => formElement.classList.remove('animate-shake'), 500)

      // Show error message with fade effect
      const errorDiv = document.createElement('div')
      errorDiv.className = 'text-red-500 text-sm mt-2 animate-fade-in font-medium'
      errorDiv.style.cssText = 'border-left: 4px solid #EF4444; padding-left: 1rem; background-color: #FEE2E2; padding: 0.75rem;'
      errorDiv.textContent = 'Invalid credentials. Please try again.'

      const submitButton = document.querySelector('button[type="submit"]')
      submitButton.parentNode.insertBefore(errorDiv, submitButton)

      // Add button shake effect
      submitButton.classList.add('animate-shake')
      setTimeout(() => submitButton.classList.remove('animate-shake'), 500)

      // Remove error message after delay with fade out
      setTimeout(() => {
        errorDiv.classList.add('animate-fade-out')
        setTimeout(() => errorDiv.remove(), 300)
      }, 3000)
    }
  }

  return (
    <div className="min-h-screen flex items-center justify-center bg-gray-50">
      <div className="max-w-md w-full space-y-8 p-8 bg-white rounded-lg shadow">
        <div className="text-center">
          <img src="/assets/images/logoIIII.svg" alt="Logo" className="mx-auto h-72 w-auto" />
          <h2 className="mt-6 text-3xl font-extrabold text-[#30825C]">Sign in to your account</h2>
        </div>
        <form className="mt-8 space-y-6" onSubmit={handleSubmit}>
          <div className="rounded-md shadow-sm -space-y-px">
            <div>
              <input
                type="text"
                required
                className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-t-md focus:outline-none focus:ring-green-500 focus:border-green-500 focus:z-10 sm:text-sm"
                placeholder="Username"
                value={credentials.username}
                onChange={(e) => setCredentials({ ...credentials, username: e.target.value })}
              />
            </div>
            <div>
              <input
                type="password"
                required
                className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-b-md focus:outline-none focus:ring-green-500 focus:border-green-500 focus:z-10 sm:text-sm"
                placeholder="Password"
                value={credentials.password}
                onChange={(e) => setCredentials({ ...credentials, password: e.target.value })}
              />
            </div>
          </div>
          <div>
            <button
              type="submit"
              className="group relative w-full flex justify-center py-2 px-4 border border-transparent text-sm font-medium rounded-md text-white bg-[#145B38] hover:bg-[#2DA066] focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-green-500"
            >
              Sign in
            </button>
          </div>
        </form>
      </div>
    </div>
  )
}

export default Login