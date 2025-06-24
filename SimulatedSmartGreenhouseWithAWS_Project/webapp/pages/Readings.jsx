import { useState, useEffect } from 'react'
import { LineChart, Line, XAxis, YAxis, Tooltip, Legend, ResponsiveContainer } from 'recharts'

const Readings = () => {
  const [activeTab, setActiveTab] = useState('sensors')
  const [sensorData, setSensorData] = useState({
    temperature: [],
    humidity: [],
    light: [],
    soilMoisture: [],
    tds: []
  })
  const [nutrientData, setNutrientData] = useState([])
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState(null)

  // Replace with your actual API Gateway endpoint
  const API_BASE_URL = 'https://obl3lvgc2e.execute-api.us-west-2.amazonaws.com/prod'

  useEffect(() => {
    const fetchSensorData = async () => {
      try {
        setLoading(true)
        const response = await fetch(`${API_BASE_URL}/sensors`)
        if (!response.ok) throw new Error('Failed to fetch sensor data')
        const data = await response.json()
        setSensorData(data)
        setError(null)
      } catch (error) {
        console.error('Error fetching sensor data:', error)
        setError('Failed to load sensor data')
      } finally {
        setLoading(false)
      }
    }

    const fetchNutrientData = async () => {
      try {
        const response = await fetch(`${API_BASE_URL}/nutrients`)
        if (!response.ok) throw new Error('Failed to fetch nutrient data')
        const data = await response.json()
        setNutrientData(data)
      } catch (error) {
        console.error('Error fetching nutrient data:', error)
        setError('Failed to load nutrient data')
      }
    }

    // Initial fetch
    fetchSensorData()
    fetchNutrientData()

    // Set up intervals for both fetches
    const sensorInterval = setInterval(fetchSensorData, 30000) // Every 30 seconds
    const nutrientInterval = setInterval(fetchNutrientData, 30000)

    // Cleanup intervals
    return () => {
      clearInterval(sensorInterval)
      clearInterval(nutrientInterval)
    }
  }, [])

  const sensors = [
    { name: 'Temperature', data: sensorData.temperature, color: '#ff7300', unit: '°C' },
    { name: 'Humidity', data: sensorData.humidity, color: '#387908', unit: '%' },
    { name: 'Light', data: sensorData.light, color: '#fbc02d', unit: 'lux' },
    { name: 'Soil Moisture', data: sensorData.soilMoisture, color: '#00bcd4', unit: '%' },
    { name: 'TDS', data: sensorData.tds, color: '#9c27b0', unit: 'ppm' }
  ]

  const formatTimestamp = (timestamp) => {
    return new Date(timestamp).toLocaleTimeString('en-US', {
      hour: '2-digit',
      minute: '2-digit'
    })
  }

  const renderSensorContent = () => (
    <div className="space-y-8">
      <div className="flex justify-between items-center">
        <h2 className="text-2xl font-bold">Sensor Readings</h2>
        {loading && <div className="text-blue-600">Loading...</div>}
        {error && <div className="text-red-600">{error}</div>}
      </div>
      
      {sensors.map((sensor) => (
        <div key={sensor.name} className="bg-white p-4 rounded-lg shadow">
          <div className="flex justify-between items-center mb-4">
            <h3 className="text-xl font-semibold">{sensor.name}</h3>
            <div className="text-sm text-gray-600">
              Latest: {sensor.data.length > 0 ? 
                `${sensor.data[0]?.value} ${sensor.unit}` : 
                'No data'}
            </div>
          </div>
          <ResponsiveContainer width="100%" height={300}>
            <LineChart data={sensor.data.slice(-20)}> {/* Show last 20 points */}
              <XAxis 
                dataKey="timestamp" 
                tickFormatter={formatTimestamp}
              />
              <YAxis />
              <Tooltip 
                labelFormatter={(value) => new Date(value).toLocaleString()}
                formatter={(value) => [`${value} ${sensor.unit}`, sensor.name]}
              />
              <Legend />
              <Line
                type="monotone"
                dataKey="value"
                stroke={sensor.color}
                name={`${sensor.name} (${sensor.unit})`}
                strokeWidth={2}
                dot={{ r: 4 }}
              />
            </LineChart>
          </ResponsiveContainer>
        </div>
      ))}
    </div>
  )

  const renderNutrientContent = () => (
    <div className="bg-white p-4 rounded-lg shadow">
      <div className="flex justify-between items-center mb-4">
        <h2 className="text-2xl font-bold">Nutrient Levels</h2>
        {loading && <div className="text-blue-600">Loading...</div>}
      </div>
      
      {nutrientData.length > 0 ? (
        <ResponsiveContainer width="100%" height={400}>
          <LineChart data={nutrientData.slice(-20)}>
            <XAxis 
              dataKey="timestamp" 
              tickFormatter={formatTimestamp}
            />
            <YAxis />
            <Tooltip 
              labelFormatter={(value) => new Date(value).toLocaleString()}
            />
            <Legend />
            <Line type="monotone" dataKey="nitrogen" stroke="#2196f3" name="Nitrogen (mg/kg)" strokeWidth={2} />
            <Line type="monotone" dataKey="phosphorus" stroke="#f44336" name="Phosphorus (mg/kg)" strokeWidth={2} />
            <Line type="monotone" dataKey="potassium" stroke="#4caf50" name="Potassium (mg/kg)" strokeWidth={2} />
          </LineChart>
        </ResponsiveContainer>
      ) : (
        <div className="text-center py-8 text-gray-500">
          No nutrient data available
        </div>
      )}
    </div>
  )

  return (
    <div className="bg-gray-50 min-h-screen p-6">
      <div className="max-w-7xl mx-auto">
        <div className="bg-white rounded-lg shadow p-6">
          <div className="mb-6">
            <div className="border-b border-gray-200">
              <nav className="-mb-px flex space-x-8">
                <button
                  className={`${
                    activeTab === 'sensors'
                      ? 'border-green-500 text-green-600'
                      : 'border-transparent text-gray-500 hover:text-gray-700 hover:border-gray-300'
                  } whitespace-nowrap py-4 px-1 border-b-2 font-medium text-sm`}
                  onClick={() => setActiveTab('sensors')}
                >
                  Sensors
                </button>
                <button
                  className={`${
                    activeTab === 'nutrients'
                      ? 'border-green-500 text-green-600'
                      : 'border-transparent text-gray-500 hover:text-gray-700 hover:border-gray-300'
                  } whitespace-nowrap py-4 px-1 border-b-2 font-medium text-sm`}
                  onClick={() => setActiveTab('nutrients')}
                >
                  Nutrients
                </button>
              </nav>
            </div>
          </div>

          <div className="mt-6">
            {activeTab === 'sensors' ? renderSensorContent() : renderNutrientContent()}
          </div>
        </div>
      </div>
    </div>
  )
}

export default Readings