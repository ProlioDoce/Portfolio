//export module shapes;
//import std;
//import <cstdint>;
//import <cmath>;
//import <numbers>;
//import mesh;
//import vec3;
//import rotation;
//import color;
//
//namespace bits
//{
//	constexpr uint8_t lpermute(uint8_t bits, uint8_t span, uint8_t shift)
//	{
//		return (bits << shift | bits >> (span - shift)) & 0xFF >> (8 - span);
//	}
//}
//
//template<uint16_t Vertices, uint16_t Faces>
//constexpr auto icosahedron(Mesh<Vertices, Faces>& mesh, float hi_phi, float pole)
//{
//	uint8_t vert_i = 0;
//
//	for (uint8_t phi_i; uint8_t pole_i : {0, 1, 2})
//	{
//		phi_i = (pole_i + 1) % 3;
//
//		for (uint8_t sign_carts : {0b00, 0b01, 0b10, 0b11})
//		{
//			auto& [pos, col] = mesh.vertices[vert_i++];
//			pos[pole_i] = sign_carts & 0b01 ? -pole : +pole;
//			pos[phi_i] = sign_carts & 0b10 ? +hi_phi : -hi_phi;
//		}
//	}
//	return vert_i;
//};
//
//export namespace shape
//{
//	template<uint8_t Vertices> requires(Vertices >= 3)
//	constexpr auto Polygon = []
//		{
//			constexpr uint8_t Faces = Vertices - 2;
//			Mesh<Vertices, Faces> mesh{};
//
//			mesh.vertices[0] = { -0.7071067811f, -0.7071067811f };
//
//			for (uint8_t i = 1; i < Vertices; ++i)
//				mesh.vertices[i] = {
//					Rotation(360.0f / Vertices, axis::Z)(mesh.vertices[i - 1].pos)
//				};
//
//			for (uint8_t i = 0; i < Faces; ++i)
//				mesh.faces[i] = { 0, uint8_t(1 + i), uint8_t(2 + i) };
//
//			return mesh;
//		}().color(color::White);
//
//	constexpr auto& Triangle = Polygon<3>;
//	constexpr auto& Square = Polygon<4>;
//	constexpr auto& Pentagon = Polygon<5>;
//
//	constexpr auto Tetrahedron	= []
//		{
//			Mesh<4, 4> mesh{};
//
//			static constexpr float unit = 0.577350269f;
//
//			for (uint8_t bit_verts = 0b000'011'101'110; uint8_t i : {0, 1, 2, 3})
//				for (auto& [pos, col] = mesh.vertices[i]; uint8_t j : {0, 1, 2})
//				{
//					mesh.faces[i][j] = (i + j) & 3;
//
//					pos[j] = -unit * ((bit_verts & 0b01) * 2 - 1);
//
//					bit_verts >>= 1;
//				}
//
//			return mesh;
//		}().color(color::White);
//	constexpr auto Cube			= []
//		{
//			Mesh<8, 12> mesh{};
//
//			static constexpr float unit = 0.577350269f;
//
//			for (uint8_t unshifted_coords, face_i = 0, pole_faces[2][4];
//				uint8_t axis_shift : {0, 1, 2 })
//			{
//				for (uint8_t plane_permtt : {0b00, 0b01, 0b10, 0b11})
//				{
//					unshifted_coords = plane_permtt << 1 | 0b001;
//					pole_faces[0][plane_permtt] = bits::lpermute(unshifted_coords, 3, axis_shift);
//					pole_faces[1][plane_permtt] = 0b001 << axis_shift ^ pole_faces[0][plane_permtt];
//
//					if (axis_shift == 0)
//						for (auto& pole : pole_faces)
//							for (auto& [pos, col] = mesh.vertices[pole[plane_permtt]]; //!!
//								uint8_t coord_shift : {0, 1, 2})
//					{
//						pos[coord_shift] = -unit * ((pole[plane_permtt] >> coord_shift & 0b01) * 2 - 1);
//					}
//				}
//				for (auto& pole : pole_faces)
//					for (uint8_t face_offs : {0, 1})
//						for (auto& face = mesh.faces[face_i++];
//							uint8_t index_i : {0, 1, 2})
//				{
//					face[index_i] = pole[index_i + face_offs];
//				}
//			}
//
//			return mesh;
//		}().color(color::White);
//	constexpr auto Octahedron	= []
//		{
//			Mesh<6, 8> mesh{};
//
//			for (uint8_t bit_ax, i = 0; float sign : { +1.0f, -1.0f })
//			{
//				for (uint8_t j : { 0, 3, 6 })
//					for (auto& [pos, col] = mesh.vertices[i++];
//						uint8_t elem_i : {0, 1, 2})
//				{
//					bit_ax = 84 >> j++ & 0b01;
//
//					pos[elem_i] = bit_ax * sign;
//					//col[elem_i] = bit_ax;
//				}
//			}
//
//			uint8_t face{};
//			for (uint8_t x : {0, 3})
//				for (uint8_t y : {1, 4})
//					for (uint8_t z : {2, 5})
//						mesh.faces[face++] = { x,y,z };
//
//			return mesh;
//		}().color(color::White);
//	constexpr auto Dodecahedron = []
//		{
//			Mesh<20, 36> mesh{};
//
//			for (uint8_t vert_i = icosahedron(mesh, 0.356822089f, 0.934172359f);
//				auto& cube_vert : Cube.vertices)
//			{
//				mesh.vertices[vert_i++] = cube_vert;
//			}
//			for (uint8_t gold_rects[3][4] = { {0,1,2,3}, {4,5,6,7}, {8,9,10, 11} },
//				cube[8] = { 12, 13, 14, 15, 16, 17, 18, 19 },
//				unshifted_coords, coords, pentagon[5], coords_offs = 2, face_i = 0;
//
//				uint8_t axis_i : {0, 2, 1})
//			{
//				auto& tip_rect = gold_rects[axis_i],
//					& supp_rect = gold_rects[(axis_i + 1) % 3];
//
//				for (uint8_t tip_i : {0b00, 0b01, 0b10, 0b11})
//				{
//					unshifted_coords = (tip_i ^ 0b10) << 1 | 0b001;
//					coords = bits::lpermute(unshifted_coords, 3, coords_offs);
//
//					pentagon[0] = tip_rect[tip_i];
//					pentagon[1] = cube[coords];
//					pentagon[2] = cube[coords ^ 0b001 << coords_offs];
//					pentagon[3] = supp_rect[0 + (0b0011 >> tip_i & 0b01)];
//					pentagon[4] = supp_rect[2 + (0b0011 >> tip_i & 0b01)];
//
//					for (uint8_t offs : {0, 1, 2})
//						mesh.faces[face_i++] = { pentagon[0 + offs], pentagon[1 + offs], pentagon[2 + offs] };
//				}; --coords_offs;
//			}
//
//			return mesh;
//		}().color(color::White);
//	constexpr auto Icosahedron	= []
//		{
//			Mesh<12, 20> mesh{};
//
//			icosahedron(mesh, 0.525731112f, 0.850650808f);
//
//			uint8_t golden_rects[3][4] = { {0,1,2,3}, {4,5,6,7}, {8,9,10,11} }, face_i = 0;
//			auto& [x, y, z] = golden_rects;
//
//			for (uint8_t togg_two = 0, y_i = 1; uint8_t x_i : {0, 2})
//			{
//				for (uint8_t z_i : {0, 1, 2, 3})
//					mesh.faces[face_i++] = {
//						x[x_i + (0b0011 >> z_i & 0b01)],
//						y[y_i + (togg_two ^= 0b10)],
//						z[z_i]
//				};  --y_i;
//			}
//
//			for (uint8_t face_i = 8; uint8_t ax_i : {0, 1, 2})
//			{
//				auto& ax_rect = golden_rects[ax_i];
//				auto& supp_rect = golden_rects[(ax_i + 2) % 3];
//
//				for (uint8_t ax_i : {0, 1})
//					for (uint8_t supp_i : {2, 3})
//						mesh.faces[face_i++] = { ax_rect[ax_i], ax_rect[ax_i + 2], supp_rect[supp_i - ax_i * 2] };
//			}
//
//			return mesh;
//		}().color(color::White);
//
//	template<uint8_t Freq> requires(Freq <= 3)
//	constexpr auto Tetrosphere = []
//		{
//			Mesh<(Freq + 2) * (Freq + 3) * (Freq + 4) / 6,
//				4 * (Freq + 1) * (Freq + 1)> mesh{};
//
//
//
//			return mesh;
//		}().color(color::White);
//	template<uint8_t Freq> requires(Freq <= 3)
//	constexpr auto Cubosphere = []	//dense_index -> 0b(taken)000'0000'0000'0000'; |--CONDENSADOR--|
//		{							//face_i = dense_indices[face_i].taken ? dense_indices[face_i].i : push_vert, (dense_indices[face_i] = {dense_i++, true}).i;
//			static constexpr uint16_t//to rather repeat edges than be branching | the snapshot indices option
//				EdgeVertices = 2 << Freq,
//				CoreEdgeVertices = EdgeVertices - 2,
//				FullVertices = (8 << Freq * 3),
//				ShelledVertices =  FullVertices - CoreEdgeVertices * CoreEdgeVertices * CoreEdgeVertices,
//				EdgeFaces = EdgeVertices - 1,
//				Faces = EdgeFaces * EdgeFaces * 12;
//
//			Mesh<ShelledVertices, Faces> mesh{};
//
//			const uint8_t
//				cell_size = Freq + 1,
//				cell_mask = EdgeFaces,
//				subdiv_offs = (1 << cell_size) - 2,
//				traversal = CoreEdgeVertices;
//			
//			const uint16_t
//				full_mask = cell_mask | cell_mask << cell_size | cell_mask << cell_size * 2,
//				face_map[4]{ 0, 1, 2u + subdiv_offs, 3u + subdiv_offs };
//			
//			const float f_scaling = 1.0f / EdgeFaces; mesh.vertices[0].pos = { -0.5f, -0.5f, -0.5f };
//			
//			for (uint16_t unshifted_coords, dense_indices[FullVertices]{}, pole_faces[2][EdgeVertices * EdgeVertices], whole_axis, face_i = 0, dense_i = 0,
//				axis_shift = 0; axis_shift < 3 * cell_size; axis_shift += cell_size)
//			{
//				for (uint16_t plane_permtt = 0; plane_permtt < EdgeVertices * EdgeVertices; ++plane_permtt)
//				{
//					unshifted_coords = plane_permtt << cell_size;
//					pole_faces[1][plane_permtt] = (unshifted_coords << axis_shift | unshifted_coords >> (cell_size * 3 - axis_shift)) & full_mask;
//					pole_faces[0][plane_permtt] = pole_faces[1][plane_permtt] ^ cell_mask << axis_shift;
//				}
//				for (auto& face : pole_faces)
//				{
//					for (auto& bit_pos : face)
//						if(auto& dense_index = dense_indices[bit_pos]; 
//							bit_pos && !dense_index)
//							for (auto& vertex = mesh.vertices[dense_index = ++dense_i];
//								uint8_t elem_i : {0, 1, 2})
//							{
//								whole_axis = bit_pos >> elem_i * cell_size & cell_mask; //0,1,2,3
//
//								vertex.pos[elem_i] = -0.5f + whole_axis * f_scaling;
//							}
//			
//					for (uint8_t height_offs = 0; height_offs < EdgeFaces * EdgeFaces; height_offs += EdgeVertices)
//						for (uint8_t axis_corner = 0; axis_corner < EdgeFaces; ++axis_corner)
//							for (uint8_t face_offs : {0, 1})
//								for (auto& tr_face = mesh.faces[face_i++];
//									uint8_t index : {0, 1, 2})
//								{
//									tr_face[index] = dense_indices[face[face_map[index + face_offs] + axis_corner + height_offs]];
//								}
//				}
//			}
//			for (auto& vertex : mesh.vertices)
//				vertex.pos.normalize();
//
//			return mesh;
//		}().color(color::White);
//}