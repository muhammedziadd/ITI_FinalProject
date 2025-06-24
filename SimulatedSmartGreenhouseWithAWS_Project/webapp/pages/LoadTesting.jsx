import { useState } from 'react'
import axios from 'axios'

const LoadTesting = () => {
  const [testResults, setTestResults] = useState(null)
  const [loading, setLoading] = useState(false)
  const [config, setConfig] = useState({
    users: 100,
    duration: 5,
    rampUp: 30
  })

  const runLoadTest = async () => {
    setLoading(true)
    try {
      const response = await axios.post('YOUR_API_ENDPOINT/load-test', config)
      setTestResults(response.data)
    } catch (error) {
      console.error('Error running load test:', error)
    } finally {
      setLoading(false)
    }
  }

  return (
    <div className="bg-white p-4 rounded-lg shadow">
      <h2 className="text-2xl font-bold mb-4">Load Testing</h2>
      
      <div className="space-y-4 mb-6">
        <div>
          <label className="block text-sm font-medium text-gray-700">Number of Users</label>
          <input
            type="number"
            value={config.users}
            onChange={(e) => setConfig({ ...config, users: parseInt(e.target.value) })}
            className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-green-500 focus:ring-green-500"
          />
        </div>
        
        <div>
          <label className="block text-sm font-medium text-gray-700">Duration (minutes)</label>
          <input
            type="number"
            value={config.duration}
            onChange={(e) => setConfig({ ...config, duration: parseInt(e.target.value) })}
            className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-green-500 focus:ring-green-500"
          />
        </div>

        <div>
          <label className="block text-sm font-medium text-gray-700">Ramp-up Period (seconds)</label>
          <input
            type="number"
            value={config.rampUp}
            onChange={(e) => setConfig({ ...config, rampUp: parseInt(e.target.value) })}
            className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-green-500 focus:ring-green-500"
          />
        </div>

        <button
          onClick={runLoadTest}
          disabled={loading}
          className="w-full bg-green-600 text-white p-2 rounded hover:bg-green-700 disabled:bg-green-300"
        >
          {loading ? 'Running Test...' : 'Start Load Test'}
        </button>
      </div>

      {testResults && (
        <div className="mt-6 p-4 bg-gray-50 rounded-lg">
          <h3 className="text-xl font-semibold mb-4">Test Results</h3>
          <div className="space-y-2">
            <p><strong>Average Response Time:</strong> {testResults.avgResponseTime}ms</p>
            <p><strong>Requests per Second:</strong> {testResults.requestsPerSecond}</p>
            <p><strong>Error Rate:</strong> {testResults.errorRate}%</p>
            <p><strong>EC2 Auto Scaling Events:</strong> {testResults.scalingEvents}</p>
          </div>
        </div>
      )}
    </div>
  )
}

export default LoadTesting