# Tetrahedron Pair Data Generator

A configurable tool for generating synthetic datasets of intersecting/non-intersecting tetrahedron pairs. Supports multiple intersection types (point, segment, polygon, polyhedron) and output formats (CSV/JSON/OBJ). Ideal for creating training/evaluation data for geometric ML models.

## Key Components

### Configuration Management
- **JSON Configuration**: Define dataset size, precision, intersection distributions, and volume ranges.
- **Validation**: Ensures parameters are valid (e.g., distributions sum to 100%, volume ranges are feasible).

### Data Writers
- **Formats**: CSV, JSON, and OBJ output via `BaseWriter` interface.
- **Dynamic Selection**: Writer chosen automatically based on configuration.

### Geometry Utilities
- **Intersection Checks**: Detects intersections between tetrahedrons.
- **Volume Computation**: Calculates intersection volumes with user-specified precision.

### Tetrahedron Factory
- **Controlled Generation**: Creates random tetrahedron pairs adhering to configured distributions (intersection types, volume ranges).

### Generation Workflow
1. Distributes workload based on intersection type ratios.
2. Generates pairs, computes intersections/volumes.
3. Writes data with real-time progress tracking.

---

## Generation Strategies

### Baseline Strategy
- **Process**:
  1. Randomly generate tetrahedron vertices.
  2. Validate tetrahedron geometry.
  3. Check for intersection and compute volume.
  4. Repeat until dataset is complete.

### No Intersection
- **Guaranteed Separation**:
  - Generate random pairs until non-intersecting pair is found.
  - Useful for creating negative training examples.

### Point Intersection
- **Precision Control**:
  1. Generate a vertex on a face of `T1`.
  2. Create `T2` with vertices projected along the face's normal.
  3. Ensure single-point contact using spherical coordinates and radius limits.

### Segment Intersection
- **Line Contact**:
  1. Project two points onto a face of `T1`.
  2. Build `T2` vertices along the face's plane to form a line intersection.
  3. Validate using spherical coordinates and radius constraints.

### Polygon Intersection
- **Planar Overlap**:
  1. Project three points onto a face of `T1`.
  2. Compute fourth vertex of `T2` along the face's normal.
  3. Ensure polygonal overlap while avoiding full containment.

### Polyhedron Intersection
- **Volume Overlap**:
  - Generate random pairs until intersecting configuration is found.
  - Most common for complex 3D overlaps.

---

## Usage

### Installation and Setup (Linux)

1. **Clone the Repository**
   ```sh
   git clone <repository-url>
   cd <repository-name>
   ```

2. **Create a Build Directory**
   ```sh
   mkdir build && cd build
   ```

3. **Install Dependencies**
   ```sh
   sudo apt update
   sudo apt install g++ libcgal-dev
   ```

4. **Set Environment Variables**
   ```sh
   export CXX=/usr/bin/g++
   export CGAL_DIR=/usr/lib/CGAL
   ```

5. **Compile the Application**
   ```sh
   cmake ..
   make
   ```

6. **Adjust Configuration File**
   Edit the configuration file at:
   ```sh
   nano tetrahedron_pair_generator/config/config.json
   ```
   Example Configuration:
   ```json
   {
     "dataset_size": 10000,
     "output_format": "csv",
     "precision": 6,
     "intersection_distribution": {
       "none": 30,
       "point": 20,
       "segment": 20,
       "polygon": 20,
       "polyhedron": 10
     }
   }
   ```

7. **Run the Generator**
   ```sh
   cd bin
   ./TetrahedronPairGenerator
   ```

8. **Generated Data**
   The output files will be stored in the specified format (CSV/JSON/OBJ) as configured in `config.json`.

---

## Troubleshooting

### CMake Cannot Find CGAL
If CMake throws an error about `FindCGAL.cmake`, manually set the CGAL directory:
```sh
cmake -DCGAL_DIR=/usr/lib/CGAL ..
```

### Compilation Errors
Ensure all dependencies are installed:
```sh
sudo apt install g++ libcgal-dev
```
If errors persist, try cleaning the build directory and recompiling:
```sh
rm -rf build/*
cmake ..
make
```
