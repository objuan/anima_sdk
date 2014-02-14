#pragma once

using namespace Ogre;

class ANIMA_CORE_API GfxHardwareBufferManager : public HardwareBufferManager
{
public:

	HardwareVertexBufferSharedPtr buff;
	HardwareIndexBufferSharedPtr idx;

	GfxHardwareBufferManager(){}

		virtual HardwareVertexBufferSharedPtr 
            createVertexBuffer(size_t vertexSize, size_t numVerts, HardwareBuffer::Usage usage, 
			bool useShadowBuffer = false);
	
		virtual HardwareIndexBufferSharedPtr 
            createIndexBuffer(HardwareIndexBuffer::IndexType itype, size_t numIndexes, 
			HardwareBuffer::Usage usage, bool useShadowBuffer = false);
};

class ANIMA_CORE_API GfxOgreRenderSystem : public Ogre::RenderSystem
{
private:
	
	ConfigOptionMap configMap;

public:

	GfxOgreRenderSystem();
	~GfxOgreRenderSystem();

	virtual const String& getName(void) const{return "";}

	virtual ConfigOptionMap& getConfigOptions(void){return configMap;}

		virtual void setConfigOption(const String &name, const String &value){}

		/** Create an object for performing hardware occlusion queries. 
		*/
		virtual HardwareOcclusionQuery* createHardwareOcclusionQuery(void) ;

		/** Destroy a hardware occlusion query object. 
		*/
		virtual void destroyHardwareOcclusionQuery(HardwareOcclusionQuery *hq){}

        /** Validates the options set for the rendering system, returning a message if there are problems.
            @note
                If the returned string is empty, there are no problems.
        */
		virtual String validateConfigOptions(void) {return "";}

        /** Start up the renderer using the settings selected (Or the defaults if none have been selected).
            @remarks
                Called by Root::setRenderSystem. Shouldn't really be called
                directly, although  this can be done if the app wants to.
            @param
                autoCreateWindow If true, creates a render window
                automatically, based on settings chosen so far. This saves
                an extra call to RenderSystem::createRenderWindow
                for the main render window.
            @par
                If an application has more specific window requirements,
                however (e.g. a level design app), it should specify false
                for this parameter and do it manually.
            @returns
                A pointer to the automatically created window, if requested, otherwise null.
        */
        virtual RenderWindow* initialise(bool autoCreateWindow, const String& windowTitle = "OGRE Render Window");

        /** Restart the renderer (normally following a change in settings).
        */
		virtual void reinitialise(void){}

        /** Shutdown the renderer and cleanup resources.
        */
        virtual void shutdown(void){}


        /** Sets the colour & strength of the ambient (global directionless) light in the world.
        */
        virtual void setAmbientLight(float r, float g, float b){}

        /** Sets the type of light shading required (default = Gouraud).
        */
        virtual void setShadingType(ShadeOptions so){}

        /** Sets whether or not dynamic lighting is enabled.
            @param
                enabled If true, dynamic lighting is performed on geometry with normals supplied, geometry without
                normals will not be displayed. If false, no lighting is applied and all geometry will be full brightness.
        */
        virtual void setLightingEnabled(bool enabled){}

        /** Sets whether or not W-buffers are enabled if they are avalible for this renderer.
			@param
				enabled If true and the renderer supports them W-buffers will be used.  If false 
				W-buffers will not be used even if avalible.  W-buffers are enabled by default 
				for 16bit depth buffers and disabled for all other depths.
        */
		void setWBufferEnabled(bool enabled){}

		/** Returns true if the renderer will try to use W-buffers when avalible.
		*/
		bool getWBufferEnabled(void) const { return false;}

	
		virtual RenderWindow* createRenderWindow(const String &name, unsigned int width, unsigned int height, 
			bool fullScreen, const NameValuePairList *miscParams = 0) ;

	
	/*	RenderTexture * createRenderTexture( const String & name, unsigned int width, unsigned int height,
		 	TextureType texType = TEX_TYPE_2D, PixelFormat internalFormat = PF_X8R8G8B8, 
			const NameValuePairList *miscParams = 0 ); */

	
			virtual MultiRenderTarget * createMultiRenderTarget(const String & name);

		virtual String getErrorDescription(long errorNumber) const {return "";} 

        ////void setWaitForVerticalBlank(bool enabled);

        /////** Returns true if the system is synchronising frames with the monitor vertical blank.
        ////*/
        ////bool getWaitForVerticalBlank(void) const;
       
		virtual void setStencilCheckEnabled(bool enabled){}

        virtual void setStencilBufferParams(CompareFunction func = CMPF_ALWAYS_PASS, 
            uint32 refValue = 0, uint32 mask = 0xFFFFFFFF, 
            StencilOperation stencilFailOp = SOP_KEEP, 
            StencilOperation depthFailOp = SOP_KEEP,
            StencilOperation passOp = SOP_KEEP, 
            bool twoSidedOperation = false){}



		/** Sets the current vertex declaration, ie the source of vertex data. */
		virtual void setVertexDeclaration(VertexDeclaration* decl) {}
		/** Sets the current vertex buffer binding state. */
		virtual void setVertexBufferBinding(VertexBufferBinding* binding){}

        /** Sets whether or not normals are to be automatically normalised.
        @remarks
            This is useful when, for example, you are scaling SceneNodes such that
            normals may not be unit-length anymore. Note though that this has an
            overhead so should not be turn on unless you really need it.
        @par
            You should not normally call this direct unless you are rendering
            world geometry; set it on the Renderable because otherwise it will be
            overridden by material settings. 
        */
        virtual void setNormaliseNormals(bool normalise) {}

		/** Gets the capabilities of the render system. */
		const RenderSystemCapabilities* getCapabilities(void) const { return mCapabilities; }

        /////** Binds a given GpuProgram (but not the parameters). 
        ////@remarks Only one GpuProgram of each type can be bound at once, binding another
        ////one will simply replace the exsiting one.
        ////*/
        ////virtual void bindGpuProgram(GpuProgram* prg);

        /** Bind Gpu program parameters.
        */
        virtual void bindGpuProgramParameters(GpuProgramType gptype, GpuProgramParametersSharedPtr params){}
   		/** Only binds Gpu program parameters used for passes that have more than one iteration rendering
        */
        virtual void bindGpuProgramPassIterationParameters(GpuProgramType gptype) {}
       
        /** sets the clipping region.
        */
        virtual void setClipPlanes(const PlaneList& clipPlanes) {}

        ///** Set a clipping plane. */
        //virtual void setClipPlane (ushort index, const Plane &p);
        /** Set a clipping plane. */
        virtual void setClipPlane (unsigned short index, Real A, Real B, Real C, Real D) {}
        /** Enable the clipping plane. */
        virtual void enableClipPlane (unsigned short index, bool enable) {}

        ///** Sets whether or not vertex windings set should be inverted; this can be important
        //    for rendering reflections. */
        //virtual void setInvertVertexWinding(bool invert);

        virtual void setScissorTest(bool enabled, size_t left = 0, size_t top = 0, 
			size_t right = 800, size_t bottom = 600) {}

        virtual void clearFrameBuffer(unsigned int buffers, 
            const ColourValue& colour = ColourValue::Black, 
			Real depth = 1.0f, unsigned short stencil = 0) {};
     
		virtual Real getHorizontalTexelOffset(void){return 0;}
        /** Returns the vertical texel offset value required for mapping 
        texel origins to pixel origins in this rendersystem.
        @remarks
        Since rendersystems sometimes disagree on the origin of a texel, 
        mapping from texels to pixels can sometimes be problematic to 
        implement generically. This method allows you to retrieve the offset
        required to map the origin of a texel to the origin of a pixel in
        the vertical direction.
        */
        virtual Real getVerticalTexelOffset(void){return 0;}

        /** Gets the minimum (closest) depth value to be used when rendering
            using identity transforms.
        @remarks
            When using identity transforms you can manually set the depth
            of a vertex; however the input values required differ per
            rendersystem. This method lets you retrieve the correct value.
        @see Renderable::getUseIdentityView, Renderable::getUseIdentityProjection
        */
        virtual Real getMinimumDepthInputValue(void){return 1;}
        /** Gets the maximum (farthest) depth value to be used when rendering
            using identity transforms.
        @remarks
            When using identity transforms you can manually set the depth
            of a vertex; however the input values required differ per
            rendersystem. This method lets you retrieve the correct value.
        @see Renderable::getUseIdentityView, Renderable::getUseIdentityProjection
        */
        virtual Real getMaximumDepthInputValue(void){return 2;}
        /** set the current multi pass count value.  This must be set prior to 
            calling _render() if multiple renderings of the same pass state are 
            required.
        @param count Number of times to render the current state.
        */
        void setCurrentPassIterationCount(const size_t count) { mCurrentPassIterationCount = count; }

		virtual void preExtraThreadsStarted() {}

		virtual void postExtraThreadsStarted(){}
		
		virtual void registerThread() {}
			
		/** Unregister an additional thread which may make calls to rendersystem-related objects.
		@see RenderSystem::registerThread
		*/
		virtual void unregisterThread(){}

		virtual VertexElementType getColourVertexElementType(void) const ;

		virtual void _convertProjectionMatrix(const Matrix4& matrix,
			Matrix4& dest, bool forGpuProgram = false){}
        /** See
          RenderSystem
         */
        virtual void _makeProjectionMatrix(const Radian& fovy, Real aspect, Real nearPlane, Real farPlane, 
            Matrix4& dest, bool forGpuProgram = false){}
        /** See
        RenderSystem
        */
        virtual void _makeProjectionMatrix(Real left, Real right, Real bottom, Real top, 
            Real nearPlane, Real farPlane, Matrix4& dest, bool forGpuProgram = false){}
        /** See
          RenderSystem
         */
		virtual void _makeOrthoMatrix(const Radian& fovy, Real aspect, Real nearPlane, Real farPlane, 
            Matrix4& dest, bool forGpuProgram = false){}

	    void _applyObliqueDepthProjection(Matrix4& matrix, const Plane& plane, 
			bool forGpuProgram){}
       
			void _setPolygonMode(PolygonMode level){}

		  // ------------------------------------------------------------------------
        //                     Internal Rendering Access
        // All methods below here are normally only called by other OGRE classes
        // They can be called by library user if required
        // ------------------------------------------------------------------------


        /** Tells the rendersystem to use the attached set of lights (and no others) 
        up to the number specified (this allows the same list to be used with different
        count limits) */
		virtual void _useLights(const LightList& lights, unsigned short limit) {}
        /** Sets the world transform matrix. */
        virtual void _setWorldMatrix(const Matrix4 &m)  {}
        /** Sets multiple world matrices (vertex blending). */
        virtual void _setWorldMatrices(const Matrix4* m, unsigned short count) {}
        /** Sets the view transform matrix */
        virtual void _setViewMatrix(const Matrix4 &m)  {}
        /** Sets the projection transform matrix */
        virtual void _setProjectionMatrix(const Matrix4 &m) {}
 
        virtual void _setSurfaceParams(const ColourValue &ambient,
            const ColourValue &diffuse, const ColourValue &specular,
            const ColourValue &emissive, Real shininess,
            TrackVertexColourType tracking = TVC_NONE)  {}

		virtual void _setPointSpritesEnabled(bool enabled)  {}

		virtual void _setPointParameters(Real size, bool attenuationEnabled, 
			Real constant, Real linear, Real quadratic, Real minSize, Real maxSize) {}

        virtual void _setTexture(size_t unit, bool enabled, 
			const TexturePtr &texPtr)  {}
       
        virtual void _setTextureCoordSet(size_t unit, size_t index)  {}

        virtual void _setTextureCoordCalculation(size_t unit, TexCoordCalcMethod m, 
            const Frustum* frustum = 0)  {}

        virtual void _setTextureBlendMode(size_t unit, const LayerBlendModeEx& bm)  {}


        virtual void _setTextureUnitFiltering(size_t unit, FilterType ftype, FilterOptions filter)  {}

		/** Sets the maximal anisotropy for the specified texture unit.*/
		virtual void _setTextureLayerAnisotropy(size_t unit, unsigned int maxAnisotropy)  {}

		/** Sets the texture addressing mode for a texture unit.*/
        virtual void _setTextureAddressingMode(size_t unit, const TextureUnitState::UVWAddressingMode& uvw)  {}

		/** Sets the texture border colour for a texture unit.*/
        virtual void _setTextureBorderColour(size_t unit, const ColourValue& colour)  {}

		virtual void _setTextureMipmapBias(size_t unit, float bias)  {}

        virtual void _setTextureMatrix(size_t unit, const Matrix4& xform)  {}

        virtual void _setSceneBlending(SceneBlendFactor sourceFactor, SceneBlendFactor destFactor)  {}

        virtual void _setAlphaRejectSettings(CompareFunction func, unsigned char value)  {}

        virtual void _beginFrame(void)  {}

        virtual void _endFrame(void)  {}

        virtual void _setViewport(Viewport *vp)  {}
       
        virtual void _setCullingMode(CullingMode mode)  {}

        virtual void _setDepthBufferParams(bool depthTest = true, bool depthWrite = true, CompareFunction depthFunction = CMPF_LESS_EQUAL)  {}

		virtual void _setDepthBufferCheckEnabled(bool enabled = true)  {}
        
		virtual void _setDepthBufferWriteEnabled(bool enabled = true)  {}
       
		virtual void _setDepthBufferFunction(CompareFunction func = CMPF_LESS_EQUAL)  {}
		
		virtual void _setColourBufferWriteEnabled(bool red, bool green, bool blue, bool alpha)  {}
       
		virtual void _setDepthBias(float constantBias, float slopeScaleBias = 0.0f)  {}
       
		virtual void _setFog(FogMode mode = FOG_NONE, const ColourValue& colour = ColourValue::White, Real expDensity = 1.0, Real linearStart = 0.0, Real linearEnd = 1.0)  {}
};
